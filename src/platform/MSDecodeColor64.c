/* MSDecodeColor64: candidate for 1070:85b2 (237 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The sibling of MSDecodeColor256 (1070:84c8) for 6-bit components: each byte is shifted
 * left by two on the way out. Unpacks a run-length palette: a count word, then for each run a position delta (in
 * entries) and a length byte (0 meaning 256), followed by length*3 RGB bytes copied to
 * dst at entry position*3. The min and max of the touched range are tracked but not used.
 *
 * RENAMED 2026-09-27 from unpack_palette_runs_x4: Mac MSDecodeColor64(unsigned char*, unsigned char*), FLIC chunk 11 (the x4 is the 6-to-8-bit scale). Arity 2. FLIC DECODER BLOCK: Windows 1070 84c8, 85b2, 86ad, 86d7, 8990, 89ba; Mac CODE 4 MSDecodeColor256, Color64, DeltaFlc, DeltaFli, ByteRun, Literal. The order is EXACT. The single caller of each is Flic_DecodeFrame (1068:1b11), in the seq position of its Mac namesake, 8 of 8 (FLIC chunks 4, 7, 11, 12, 15, 16). NOT PROPOSED BY mac_bind. Sizes differ by implementation: the 42-byte Windows ss2/brun/copy are thunks to assembly, and the Mac DeltaFli is its 40-byte thunk. Arity is the tie-break and agrees on every one.
 */

#include "colonization.h"

void MSDecodeColor64(unsigned char *src, unsigned char *dst)
{
    short pos = 0;
    short count = *(short *)src;
    short base;
    short min;
    short max;
    short i;
    short run;

    ((unsigned char *)src) += 2;
    min = 32000;
    max = -1;
    while (--count >= 0) {
        pos += *src++;
        if ((run = *src++) == 0)
            run = 0x100;
        if (min > pos)
            min = pos;
        if (pos + run > max)
            max = pos + run;
        for (i = 0, base = pos * 3; run * 3 > i; i++)
            dst[base + i] = *src++ << 2;
    }
}
