/* PORT: the game's 16-bit words read and written as i16, not int (4 bytes here) (W4). */
/* MSDecodeDeltaFli: candidate for 1070:86d7 (697 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws a run-length sprite into a bitmap of `stride` bytes per row, adding `add` to every
 * pixel. The source begins with a first-row offset and a row count; each row is a count of
 * runs, each run a skip byte then a signed length: positive copies that many bytes (dwords
 * first, with the offset replicated into all four lanes), negative repeats one byte -length
 * times (again dwords first).
 *
 * RENAMED 2026-09-26 from blit_rle_offset: the FLIC DELTA_FLI (chunk 12, "LC") decoder: a first-line offset
 * and a line count, then per line a packet count and packets of skip byte + signed run -- positive
 * copies, negative repeats -- exactly the LC layout.
 *
 * RENAMED 2026-09-27 from flic_decode_lc: Mac MSDecodeDeltaFli(unsigned char*, unsigned char*, short, unsigned char), FLIC chunk 12 (LC). Arity 4 = (src, dst, stride, add). FLIC DECODER BLOCK: Windows 1070 84c8, 85b2, 86ad, 86d7, 8990, 89ba; Mac CODE 4 MSDecodeColor256, Color64, DeltaFlc, DeltaFli, ByteRun, Literal. The order is EXACT. The single caller of each is Flic_DecodeFrame (1068:1b11), in the seq position of its Mac namesake, 8 of 8 (FLIC chunks 4, 7, 11, 12, 15, 16). NOT PROPOSED BY mac_bind. Sizes differ by implementation: the 42-byte Windows ss2/brun/copy are thunks to assembly, and the Mac DeltaFli is its 40-byte thunk. Arity is the tie-break and agrees on every one.
 */

#include "colonization.h"

void MSDecodeDeltaFli(unsigned char *src, unsigned char *dst, int stride, unsigned char add)
{
    int col0 = 0;
    int row0 = 0;
    unsigned char *p;
    int rows;
    unsigned char count;
    signed char n;
    unsigned char rem;
    signed char dwords;
    long v;
    unsigned long add32;
    unsigned add16;
    int col;
    int row;

    add16 = add << 8;
    add16 += add;
    add32 = (unsigned long)add16 << 16;
    add32 += add16;
    row = *(i16 *)src + row0;
    src += 2;
    rows = *(i16 *)src;
    src += 2;
    while (--rows >= 0) {
        col = col0;
        count = *src;
        src++;
        p = dst + (long)row * stride;
        while (count > 0) {
            col += *src;
            p += *src;
            src++;
            n = *src;
            src++;
            if ((int)n < 0) {
                n = -n;
                rem = n & 3;
                dwords = n >> 2;
                v = (long)(*src + add) + ((long)(*src + add) << 8) + ((long)(*src + add) << 16) + ((long)(*src + add) << 24);
                while (dwords-- != 0) {
                    *(long *)p = v;
                    p += 4;
                }
                while (rem-- != 0) {
                    *p = *src + add;
                    p++;
                }
                src++;
                col += n;
                count--;
            } else {
                rem = n & 3;
                dwords = n >> 2;
                while (dwords-- != 0) {
                    *(long *)p = *(long *)src + add32;
                    src += 4;
                    p += 4;
                }
                while (rem-- != 0) {
                    *p = *src + add;
                    src++;
                    p++;
                }
                col += n;
                count--;
            }
        }
        row++;
    }
}
