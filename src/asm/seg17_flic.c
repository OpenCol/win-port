/* Segment 17, in C (roadmap W5): the FLIC chunk decoders asm_decode_delta_flc (1080:0000),
 * asm_decode_byte_run (1080:00e4), asm_decode_literal (1080:0168), and the random number
 * routines at 1080:01ac..0205 (L17_01dc, L17_01f0, L17_01fe).
 *
 * Written from the shipped routines block by block, so that W5's differential can hold them to
 * the originals, quirks included -- each kept quirk is marked. `add` is added to every pixel
 * (the FLIC's palette offset).
 */
#include <stdint.h>

static uint16_t rd16(const unsigned char *p) { return (uint16_t)(p[0] | p[1] << 8); }

/* 1080:0000, DELTA_FLC (chunk 7): a line count, then per line an opcode word -- negative: skip
   that many lines; else that many packets of {skip byte, signed count byte, data}: a positive
   count copies 2*count bytes, a negative one repeats a pixel pair -count times. */
void asm_decode_delta_flc(intptr_t src_l, intptr_t dst_l, int c, int d, int stride, unsigned char add)
{
    const unsigned char *s = (const unsigned char *)(intptr_t)src_l;
    unsigned char *o = (unsigned char *)(intptr_t)dst_l;
    uint16_t lines = rd16(s);
    (void)c;
    (void)d;
    s += 2;
    for (;;) {
        uint16_t op = rd16(s);
        s += 2;
        if ((int16_t)op < 0) {
            /* quirk kept: FLC's "last pixel" opcode (high bits 10) is not told apart -- every
               negative word skips lines, and the 0x4000 test below it can never hold */
            if (op != 0x4000) {
                o += (uint32_t)(uint16_t)(-(int16_t)op) * (uint16_t)stride;
                continue;
            }
            o += (uint16_t)stride;
            o[-1] = (unsigned char)op;
            if (--lines == 0)
                return;
            continue;
        }
        {
            uint16_t x = 0, packets = op;          /* bp-2 */
            /* tested `< 0` before a packet and `!= 0` after it: a count of 0 is one packet */
            do {
                int16_t count;
                o += *s;
                x = (uint16_t)(x + *s++);
                count = (int8_t)*s++;
                if (count >= 0) {
                    /* `add cl,cl`: the count doubled in its low byte */
                    uint16_t n = (uint16_t)(((uint16_t)count & 0xff00) | (uint8_t)(count * 2));
                    x = (uint16_t)(x + n);
                    do {
                        *o++ = (unsigned char)(*s++ + add);
                    } while (--n);
                } else {
                    uint16_t n = (uint16_t)(-count);
                    x = (uint16_t)(x + 2 * n);
                    do {
                        o[0] = (unsigned char)(s[0] + add);
                        o[1] = (unsigned char)(s[1] + add);
                        o += 2;
                    } while (--n);
                    s += 2;
                }
            } while ((int16_t)--packets > 0);
            /* the line's end: stride less what was written, a 16-bit difference zero-extended */
            o += (uint16_t)((uint16_t)stride - x);
        }
        if (--lines == 0)
            return;
    }
}

/* 1080:00e4, BYTE_RUN (chunk 15): h rows of w pixels, each row a packet count byte (ignored)
   then signed counts: positive repeats the next byte, negative copies that many bytes. */
void asm_decode_byte_run(intptr_t src_l, intptr_t dst_l, int w, int h, int stride, unsigned char add)
{
    const unsigned char *s = (const unsigned char *)(intptr_t)src_l;
    unsigned char *o = (unsigned char *)(intptr_t)dst_l;
    uint32_t gap = (uint16_t)((uint16_t)stride - (uint16_t)w);
    int16_t rows = (int16_t)h;
    while (--rows >= 0) {
        int16_t left = (int16_t)w;
        s++;
        while (left > 0) {
            int16_t count = (int8_t)*s++;
            if (count >= 0) {
                unsigned char v = (unsigned char)(*s + add);
                uint16_t n = (uint16_t)count;
                left = (int16_t)(left - count);
                do
                    *o++ = v;
                while (--n);
                s++;
            } else {
                uint16_t n = (uint16_t)(-count);
                left = (int16_t)(left - n);
                do
                    *o++ = (unsigned char)(*s++ + add);
                while (--n);
            }
        }
        /* quirk kept: the row gap is added to the source as well as the destination */
        s += gap;
        o += gap;
    }
}

/* 1080:0168, FLI_COPY (chunk 16): h rows of w raw pixels. The original moves the destination on
   by EDX after a row, a register it never sets (its caller's leftovers); the C moves it by
   stride - w, the same gap as the source, which is what a copy means. W5's differential
   decides. */
void asm_decode_literal(intptr_t src_l, intptr_t dst_l, int w, int h, int stride, unsigned char add)
{
    const unsigned char *s = (const unsigned char *)(intptr_t)src_l;
    unsigned char *o = (unsigned char *)(intptr_t)dst_l;
    uint32_t gap = (uint16_t)((uint16_t)stride - (uint16_t)w);
    uint16_t rows = (uint16_t)h;
    do {
        uint16_t n = (uint16_t)w;
        do
            *o++ = (unsigned char)(*s++ + add);
        while (--n);
        s += gap;
        o += gap;
    } while (--rows);
}

/* 1080:01ac: the generator, a 32-bit LCG at DGROUP:0x3f00 (seed = seed * 0x41c64e6d + 0x3039),
   giving the new seed's high word. Its seed has no global in the model; the port's is here. */
uint32_t g_dgroup_3f00_seed;   /* DGROUP:0x3f00 */

static uint16_t next_random(void)
{
    g_dgroup_3f00_seed = g_dgroup_3f00_seed * 0x41c64e6du + 0x3039u;
    return (uint16_t)(g_dgroup_3f00_seed >> 16);
}

/* 1080:01dc: a random number below n (0 when n is 0). The original reads n through its caller's
   frame -- [bp+6] with no frame of its own: random_below's first argument, which is this n. */
int L17_01dc(int n)
{
    if ((uint16_t)n == 0)
        return 0;
    return (int)(uint16_t)(next_random() % (uint16_t)n);
}

/* 1080:01f0: set the seed (its caller's first argument, a long). A lockstep run (W6) hands the
   seed to a filter: the oracle's, not the clock's. */
long (*port_l17_seed_filter)(long);

void L17_01f0(long seed)
{
    if (port_l17_seed_filter)
        seed = port_l17_seed_filter(seed);
    g_dgroup_3f00_seed = (uint32_t)seed;
}

/* 1080:01fe: the seed */
long L17_01fe(void)
{
    return (long)g_dgroup_3f00_seed;
}
