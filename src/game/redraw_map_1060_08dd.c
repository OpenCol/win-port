/* redraw_map_1060_08dd -- shipped at 1060:08dd.
 *
 * Ported by tools/port.py from matched/game/redraw_map_1060_08dd.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_view_nation;                        SEG20:0x7798
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void get_window_rect_1060_61dd(Win *w, RECT *r);

extern void port_set_clip(Win *w, RECT *r);
extern void port_apply_rect(Win *w);
extern void draw_map_overview(Win *w, col_i16 x, col_i16 y, col_i16 cols, col_i16 rows, col_i16 viewer,
                              col_i16 flags);

/* same object: 4-byte `push cs; call ` */


void redraw_map_1060_08dd(Win *w)
{
    get_window_rect_1060_61dd(w, &(*((RECT *)((unsigned char *)(w) + 0x77e))));
    port_set_clip(w, &(*((RECT *)((unsigned char *)(w) + 0x77e))));
    draw_map_overview(w, 0, 0, (*g_map_width), g_map_h,
                  g_reveal_all != 0 ? -1 : g_view_nation, 0);
    port_apply_rect(w);
}
