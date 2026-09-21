/* text_widest_line -- shipped at 1068:cbf3.
 *
 * Ported by tools/port.py from matched/gfx/text_widest_line.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_0_1068_dd1e(void *font);

extern col_i16 text_extent(col_i16 h, char *s);

/* same object: 4-byte `push cs; call ` */


col_i16 text_widest_line(char *s, col_i16 *pw, void *font)
{
    col_i16 lines;
    col_i16 maxw;
    col_i16 w;
    char *p;
    char buf[0x80];
    register col_i16 i;

    p = s;
    lines = 1;
    i = 0;
    maxw = 0;
    while (*p != 0) {
        if ((col_i16)*p == '_') {
            buf[i] = 0;
            w = text_extent(get_w_0_1068_dd1e(font), buf);
            if (maxw < w)
                maxw = w;
            i = 0;
            lines++;
        }
        buf[i] = *p;
        p++;
        i++;
    }
    *pw = maxw;
    return lines;
}
