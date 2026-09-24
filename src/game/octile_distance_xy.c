/* octile_distance_xy -- shipped at 1040:7b96.
 *
 * Ported by tools/port.py from matched/game/octile_distance_xy.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 octile_distance(col_i16 a, col_i16 b);

col_i16 octile_distance_xy(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2)
{
    col_i16 dx;
    col_i16 dy;

    dx = x1 - x2 > 0 ? x1 - x2 : ~(x1 - x2) + 1;
    dy = y1 - y2 > 0 ? y1 - y2 : ~(y1 - y2) + 1;
    return octile_distance(dx, dy);
}
