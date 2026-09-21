/* text_line_count -- shipped at 1068:cc91.
 *
 * Ported by tools/port.py from matched/gfx/text_line_count.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_0_1068_dd1e(void *font);

extern col_i16 text_extent(col_i16 h, char *s);

/* same object: 4-byte `push cs; call ` */


col_i16 text_line_count(char *s, col_i16 width, void *font)
{
    col_i16 w;

    w = text_extent(get_w_0_1068_dd1e(font), s);
    return w / width + 1;
}
