/* blit_rle_mask_words -- shipped at 1070:89e4.
 *
 * Ported by tools/port.py from matched/platform/blit_rle_mask_words.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 F_LXMUL(col_i32 a, col_i32 b);

void blit_rle_mask_words(unsigned char *dst, unsigned char *src, col_i16 stride)
{
    register col_i16 m;
    col_i16 col0 = 0;
    col_i16 col;
    col_i16 row;
    col_i16 rows;
    col_i16 n;
    unsigned char *p;
    col_i16 k;

    rows = *(col_i16 *)src;
    src += 2;
    row = 0;
    col = 0;
    while (rows != 0) {
        n = *(col_i16 *)src;
        src += 2;
        p = dst + (col_i32)row * stride;
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
            k = (signed char)*src;
            src++;
            if (k >= 0) {
                m = k * 2;
                while (m-- != 0) {
                    *p = 0xff;
                    p++;
                    src++;
                }
                k += k;
                col += k;
            } else {
                k = -k;
                m = k * 2;
                while (m-- != 0) {
                    *p = 0xff;
                    p++;
                }
                k += k;
                src += 2;
                col += k;
            }
            n--;
        }
        row++;
        rows--;
    }
}
