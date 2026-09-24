/* mask_runs_fill -- shipped at 1070:8ba7.
 *
 * Ported by tools/port.py from matched/platform/mask_runs_fill.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 F_LXMUL(col_i32 a, col_i32 b);

void mask_runs_fill(unsigned char *dst, unsigned char *src, col_i16 width, col_i16 rows, col_i16 stride)
{
    col_i16 ww = width;
    col_i16 r = rows;
    signed char n;
    unsigned char count;
    col_i16 i = 0;
    col_i16 x = ww;
    unsigned char *p;
    col_i16 w;

    while (--r >= 0) {
        p = dst + (col_i32)i * stride;
        w = n = count = 0;
        src++;
        while ((w += n) < x) {
            n = count = *src;
            src++;
            if ((col_i16)n >= 0) {
                while (count-- != 0) {
                    *p = 0xff;
                    p++;
                }
                src++;
            } else {
                n = count = -n;
                while (count-- != 0) {
                    *p = 0xff;
                    p++;
                    src++;
                }
            }
        }
        i++;
    }
}
