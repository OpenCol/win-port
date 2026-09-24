/* redraw_tile -- shipped at 1038:b1a0.
 *
 * Ported by tools/port.py from matched/game/redraw_tile.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void map_scroll_to_show(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2, col_i16 f);

void redraw_tile(col_i16 x, col_i16 y)
{
    map_scroll_to_show(x, y, x, y, 0);
    return;
}
