/* asm_lzw_decode: 1088:0000 (1004 bytes), the game's GIF LZW decoder, in C (roadmap W5).
 *
 * Written from the shipped routine, instruction block by instruction block (the offsets in the
 * comments are the routine's), so that W5's differential can hold it to the original byte for
 * byte. A 386 routine: it walks the source and the output with 32-bit offsets.
 *
 * The tables are MSPicDecompress's 0x6000-byte block: prefix, suffix and length, 0x1000 words
 * each. The routine indexes them with the code doubled (a byte offset into a word table), so a
 * prefix holds a doubled code or 0xffff, and only the suffix word's low byte is ever read or
 * written. Codes come least-significant bit first from length-prefixed sub-blocks. Strings are
 * written backwards from their end; the output runs on contiguously, and at the top of each
 * code the part of the last string past the row's end is moved on by stride - w, one row at a
 * time. It returns why it stopped (AX): 1 a code past the next free one, 2 end of information,
 * 3/4/5 a string that would pass the end of the bitmap.
 */
#include <stdint.h>

int asm_lzw_decode(long src_l, long bits_l, long tables_l, int w_i, int h_i, int stride_i, int code_size_i)
{
    unsigned char *src = (unsigned char *)(intptr_t)src_l;
    unsigned char *bits = (unsigned char *)(intptr_t)bits_l;
    uint16_t *prefix = (uint16_t *)(intptr_t)tables_l;
    uint16_t *suffix = prefix + 0x1000;
    uint16_t *length = prefix + 0x2000;
    int16_t w = (int16_t)w_i, h = (int16_t)h_i, stride = (int16_t)stride_i;
    uint16_t code_size = (uint16_t)code_size_i;
#define AT(t, c) ((t)[(uint16_t)(c) >> 1])      /* a word table at a doubled code */
    int16_t x = 0;                                     /* bp-8: bytes into the current row */
    int16_t gap = (int16_t)(stride - w);               /* bp-0a */
    unsigned char *pos = src, *end = src;              /* bp-1a, bp-1e: the sub-block */
    unsigned char *out = bits;                         /* bp-0e */
    unsigned char *out_end = bits + (uint32_t)(uint16_t)h * (uint16_t)stride + 1;   /* bp-16 */
    uint16_t clear = (uint16_t)(2u << code_size);      /* bp-30, doubled like every code */
    uint16_t prev = clear;                             /* bp-32 */
    uint16_t eoi = (uint16_t)(clear + 2);              /* bp-34 */
    uint16_t next = (uint16_t)(eoi + 2);               /* bp-36 */
    uint16_t init_bits = (uint16_t)(code_size + 1);    /* bp-28 */
    uint16_t cur_bits = init_bits;                     /* bp-26 */
    uint16_t init_mask = (uint16_t)((2u << init_bits) - 2);   /* bp-2c */
    uint16_t mask = init_mask;                         /* bp-2a */
    uint32_t buf = 0;                                  /* bp-22 */
    uint16_t nbits = 0;                                /* bp-24 */
    uint16_t code, c;                                  /* bp-2e */
    unsigned char *tail, *edi;                         /* bp-12, the write cursor */

    for (;;) {
        /* 0x98: a row overflowed: move what is past its end on by the gap */
        if (x >= w) {
            x = (int16_t)(x - w);
            if (gap != 0) {
                unsigned char *s = out, *d = out + gap;
                uint16_t n = (uint16_t)x;
                out = d;
                while (n--)
                    *--d = *--s;
            }
        }
        /* 0xe5: the next code */
        code = (uint16_t)buf;
        while (nbits < cur_bits) {
            if ((uint32_t)(end - pos) == 0) {          /* 0xf8: a new sub-block */
                uint32_t len = *pos++;
                end = pos + len;
            }
            buf |= (uint32_t)*pos++ << nbits;
            code = (uint16_t)buf;
            nbits = (uint16_t)(nbits + 8);
        }
        code = (uint16_t)((code << 1) & mask);         /* 0x153 */
        if (code > next)
            return 1;
        buf >>= cur_bits;
        nbits = (uint16_t)(nbits - cur_bits);
        if (code == eoi)
            return 2;
        if (code == clear) {
            cur_bits = init_bits;
            mask = init_mask;
            prev = clear;
            next = (uint16_t)(eoi + 2);
            continue;
        }
        if (prev == clear) {                           /* 0x1c7: the first code after a clear */
            out += AT(length, code);
            tail = out;
            x = (int16_t)(x + AT(length, code));
            if (out >= out_end)
                return 3;
            prev = code;
            c = code;
            do {
                *--tail = (unsigned char)AT(suffix, c);
                c = AT(prefix, c);
            } while (c != 0xffff);
            continue;
        }
        /* 0x250: a new entry: the previous string, one longer */
        AT(prefix, next) = prev;
        AT(length, next) = (uint16_t)(AT(length, prev) + 1);
        if (code < next) {                             /* 0x279: a known code */
            out += AT(length, code);
            tail = out;
            x = (int16_t)(x + AT(length, code));
            if (out >= out_end)
                return 4;
            prev = code;
            c = code;
            do {
                edi = --tail;
                *edi = (unsigned char)AT(suffix, c);
                c = AT(prefix, c);
            } while (c != 0xffff);
        } else {                                       /* 0x302: the code being defined (KwKwK) */
            uint32_t len = (uint32_t)AT(length, prev) + 1;
            out += len;
            tail = out - 1;
            x = (int16_t)(x + (int16_t)len);
            if (out >= out_end)
                return 5;
            c = prev;                                  /* the walk reuses bp-32 */
            do {
                edi = --tail;
                *edi = (unsigned char)AT(suffix, c);
                c = AT(prefix, c);
                prev = c;
            } while (c != 0xffff);
            prev = code;                               /* 0x386 */
            out[-1] = *edi;                            /* the string's first byte, again, at its end */
        }
        /* 0x3a4: the new entry's last byte is this string's first */
        AT(suffix, next) = (uint16_t)((AT(suffix, next) & 0xff00) | *edi);
        next = (uint16_t)(next + 2);
        if (next > mask && cur_bits != 12) {
            cur_bits++;
            mask = (uint16_t)(mask * 2 + 2);
        }
    }
#undef AT
}
