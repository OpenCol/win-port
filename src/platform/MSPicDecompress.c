/* MSPicDecompress: candidate for 1078:0000 (239 bytes).  Verdict: RELOCATABLE.
 *
 * Decompresses a picture through the assembly decoder asm_lzw_decode with a 0x6000-byte
 * work block: three 0x1000-word tables -- all -1, base + index, all 1 -- prepared first.
 * 0 when the block cannot be allocated (reported: "Error: Cannot allocate decompression tables in MSPicDecompress"), else 1.
 *
 * The three tables are an LZW dictionary's: 0x1000 entries is the 12-bit code space, and each
 * root code starts with no prefix (-1), itself as the suffix (base + i) and length 1. The callers
 * are load_gif_file, load_gif_resource and GRPort_LoadPicture, so asm_lzw_decode -- one of the
 * unreconstructed ENTER-prologue routines -- is the GIF LZW decoder. (2026-09-26)
 *
 * THE SIGNATURE (2026-10-02): (pixels, first colour, code size, stride, w, h, bits), the callers'
 * seven arguments. It had been read as (long, ..., void *e, long): the body pushes `bits`
 * and the source with `push dword` (66 ff 76 14), which -3 does for a long, while a pointer
 * goes as two word pushes -- so the pointers are pointers here and are cast to long only at the
 * decoder call, and `e` was the two ints w and h. One declaration now serves the definition and
 * all three callers, so the objects link (`first` is a byte on both sides: the callers' `push di`
 * is the same for an unsigned char parameter).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int MSPicDecompress(void *src, unsigned char base, int code_size, int stride, int w, int h, void *bits)
{
    /* PORT: u16, Borland's unsigned: three tables of 0x1000 words in 0x6000 bytes, which the
       assembly decoder reads as words (W4) */
    u16 *t0;
    u16 *t1;
    u16 *t2;
    unsigned hnd;
    int i;

    hnd = MSAllocateHandle(0x6000L);
    if (hnd == 0) {
        stub_reporter(&g_error_log, "Error: Cannot allocate decompression tables in MSPicDecompress");
        return 0;
    }
    t0 = (u16 *)MSHLock(hnd);
    t1 = (u16 *)((char *)t0 + 0x2000);
    t2 = (u16 *)((char *)t0 + 0x4000);
    for (i = 0; i < 0x1000; i++) {
        t0[i] = 0xffff;
        t1[i] = base + i;
        t2[i] = 1;
    }
    asm_lzw_decode((long)src, (long)bits, (long)t0, w, h, stride, code_size);
    MSHUnlock(hnd);
    MSDisposeHandle(hnd);
    return 1;
}
