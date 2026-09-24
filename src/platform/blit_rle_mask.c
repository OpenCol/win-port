/* blit_rle_mask -- shipped at 1070:8ccc.
 *
 * Ported by tools/port.py from matched/platform/blit_rle_mask.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 F_LXMUL(col_i32 a, col_i32 b);

void blit_rle_mask(unsigned char *dst, unsigned char *src, col_i16 stride)
{
    col_i16 col0 = 0;
    col_i16 row0 = 0;
    unsigned char *p;
    col_i16 rows;
    unsigned char count;
    signed char n;
    signed char k;
    col_i16 col;
    col_i16 row;

    row = *(col_i16 *)src + row0;
    src += 2;
    rows = *(col_i16 *)src;
    src += 2;
    while (--rows >= 0) {
        col = col0;
        count = *src;
        src++;
        p = dst + (col_i32)row * stride;
        while (count > 0) {
            col += *src;
            p += *src;
            src++;
            n = *src;
            src++;
            if ((col_i16)n < 0) {
                n = -n;
                k = n;
                while ((col_i16)k-- > 0) {
                    *p = 0xff;
                    p++;
                }
                src++;
                col += n;
                count = count - 1;
            } else {
                k = n;
                while ((col_i16)k-- > 0) {
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
