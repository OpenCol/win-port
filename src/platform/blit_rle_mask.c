/* PORT: the game's 16-bit words read and written as i16, not int (4 bytes here) (W4). */
/* blit_rle_mask: candidate for 1070:8ccc (430 bytes).  Verdict: RELOCATABLE.
 *
 * The mask companion of MSDecodeDeltaFli: the same run-length format, but every pixel a
 * run covers -- copied or repeated -- is written as 0xff, and the source is only skipped.
 * The run counter is spelled `count = count - 1` (ADD AL,-1), not `count--`.
 */

#include "colonization.h"

void blit_rle_mask(unsigned char *dst, unsigned char *src, int stride)
{
    int col0 = 0;
    int row0 = 0;
    unsigned char *p;
    int rows;
    unsigned char count;
    signed char n;
    signed char k;
    int col;
    int row;

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
                k = n;
                while ((int)k-- > 0) {
                    *p = 0xff;
                    p++;
                }
                src++;
                col += n;
                count = count - 1;
            } else {
                k = n;
                while ((int)k-- > 0) {
                    *p = 0xff;
                    p++;
                }
                src += n;
                col += n;
                count = count - 1;
            }
        }
        row++;
    }
}
