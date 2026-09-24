/* set_map_view_mode -- shipped at 1008:ccf6.
 *
 * Ported by tools/port.py from matched/game/set_map_view_mode.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cursor_x;                               DGROUP:0x4b30
 *   extern int g_cursor_y;                               DGROUP:0x4b32
 *   extern int far g_zoom;                               DGROUP:0x1c9a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1008_ce9c(col_i16 a, col_i16 b);
col_i16 min_int_1008_ce76(col_i16 a, col_i16 b);

extern col_i16 map_scroll_to_show(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2, col_i16 f);
extern void redraw_map_1040_11b7(col_i16 full);

/* same object: 4-byte `push cs; call ` */



void set_map_view_mode(col_i16 mode)
{
    register col_i16 v;

    v = mode;
    v = max_int_1008_ce9c(0, v);
    v = min_int_1008_ce76(3, v);
    g_zoom = v;
    if (map_scroll_to_show(g_cursor_x, g_cursor_y, g_cursor_x, g_cursor_y, 0) == 0)
        redraw_map_1040_11b7(1);
}
