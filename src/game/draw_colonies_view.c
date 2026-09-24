/* draw_colonies_view -- shipped at 1040:0b2f.
 *
 * Ported by tools/port.py from matched/game/draw_colonies_view.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern int far g_view_w;                             DGROUP:0x4c50
 *   extern int far g_view_h;                             DGROUP:0x4c52
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void draw_colonies_in_rect(col_i16 x, col_i16 y, col_i16 w, col_i16 h);

void draw_colonies_view(void)
{
    draw_colonies_in_rect(g_view_x0, g_view_y0, g_view_w, g_view_h);
}
