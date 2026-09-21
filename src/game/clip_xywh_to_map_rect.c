/* clip_xywh_to_map_rect -- shipped at 1040:0635.
 *
 * Ported by tools/port.py from matched/game/clip_xywh_to_map_rect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern int far g_view_x1;                            DGROUP:0x4c48
 *   extern int far g_view_y1;                            DGROUP:0x4c4a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1040_445d(col_i16 a, col_i16 b);
col_i16 min_int_1040_4483(col_i16 a, col_i16 b);

/* same object: 4-byte `push cs; call ` */



void clip_xywh_to_map_rect(col_i16 *x, col_i16 *y, col_i16 *w, col_i16 *h)
{
    register col_i16 right;
    register col_i16 bottom;

    right = *x + *w - 1;
    bottom = *y + *h - 1;
    *x = max_int_1040_445d(*x, g_view_x0);
    *y = max_int_1040_445d(*y, g_view_y0);
    right = min_int_1040_4483(right, g_view_x1);
    bottom = min_int_1040_4483(bottom, g_view_y1);
    *w = right - *x + 1;
    *h = bottom - *y + 1;
    *w = max_int_1040_445d(0, *w);
    *h = max_int_1040_445d(0, *h);
}
