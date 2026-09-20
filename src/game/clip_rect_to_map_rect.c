/* clip_rect_to_map_rect -- shipped at 1040:05b3.
 *
 * Ported by tools/port.py from matched/game/clip_rect_to_map_rect.c.
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

/* same object: 4-byte `push cs; call ` (rule 47) */


void clip_rect_to_map_rect(col_i16 *l, col_i16 *t, col_i16 *r, col_i16 *b)
{
    *l = max_int_1040_445d(*l, g_view_x0);
    *t = max_int_1040_445d(*t, g_view_y0);
    *r = min_int_1040_4483(*r, g_view_x1);
    *b = min_int_1040_4483(*b, g_view_y1);
}
