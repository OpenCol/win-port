/* PORT: the game's 16-bit words read and written as i16, not int (4 bytes here) (W4). */
/* blit_rle_mask_words: candidate for 1070:89e4 (451 bytes).  Verdict: RELOCATABLE.
 *
 * A third run-length mask blitter: a row count, then per row a signed word of runs. Zero
 * skips the row; a negative word with bit 0x4000 skips -n rows forward, otherwise marks one
 * pixel at the current column; a positive word is that many runs of skip byte plus signed
 * length, each marking twice the length in 0xff (and consuming as many source bytes when
 * positive). Continues without advancing the row after a negative word, as shipped.
 */

#include "colonization.h"

void blit_rle_mask_words(unsigned char *dst, unsigned char *src, int stride)
{
    register int m;
    int col0 = 0;
    int col;
    int row;
    int rows;
    int n;
    unsigned char *p;
    int length;

    rows = *(i16 *)src;
    src += 2;
    row = 0;
    col = 0;
    while (rows != 0) {
        n = *(i16 *)src;
        src += 2;
        p = dst + (long)row * stride;
        if (n == 0) {
            row++;
            rows--;
            continue;
        }
        if (n < 0) {
            if (n & 0x4000)
                row -= n;
            else {
                p += col;
                *p = 0xff;
            }
            continue;
        }
        col = col0;
        while (n > 0) {
            col += *src;
            p += *src;
            src++;
            length = (signed char)*src;
            src++;
            if (length >= 0) {
                m = length * 2;
                while (m-- != 0) {
                    *p = 0xff;
                    p++;
                    src++;
                }
                length += length;
                col += length;
            } else {
                length = -length;
                m = length * 2;
                while (m-- != 0) {
                    *p = 0xff;
                    p++;
                }
                length += length;
                src += 2;
                col += length;
            }
            n--;
        }
        row++;
        rows--;
    }
}
