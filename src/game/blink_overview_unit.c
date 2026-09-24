/* blink_overview_unit -- shipped at 1060:0fdb.
 *
 * Ported by tools/port.py from matched/game/blink_overview_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void get_window_rect_1060_61dd(void *w, RECT *r);

typedef struct { char pad[0x77e]; RECT overview_rect; } OverviewWin;





extern void port_set_clip(OverviewWin *w, RECT *r);
extern void port_apply_rect(OverviewWin *w);
extern void draw_active_unit_cell(OverviewWin *w, col_i16 x,
                                                 col_i16 y, col_i16 cols, col_i16 rows,
                                                 col_i16 viewer, col_i16 white,
                                                 col_i16 blink);

/* same object: 4-byte `push cs; call ` (rule 47) */


void blink_overview_unit(OverviewWin *w, col_i16 blink)
{
    get_window_rect_1060_61dd(w, &w->overview_rect);
    port_set_clip(w, &w->overview_rect);
    draw_active_unit_cell(w, 0, 0, (*g_map_width), g_map_h,
                                    g_reveal_all != 0 ? -1 : g_view_nation,
                                    0, blink);
    port_apply_rect(w);
}
