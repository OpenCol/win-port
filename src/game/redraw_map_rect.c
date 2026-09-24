/* redraw_map_rect -- shipped at 1060:096c.
 *
 * Ported by tools/port.py from matched/game/redraw_map_rect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_view_tile_w;                            DGROUP:0x4e0c
 *   extern int g_view_tile_h;                            DGROUP:0x4e0e
 *   extern int g_view_org_x;                             DGROUP:0x4e10
 *   extern int g_view_org_y;                             DGROUP:0x4e12
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
void copy_rect_to_screen_1060_631c(Win *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

/* same object: 4-byte `push cs; call ` */


extern void draw_map_overview(Win *w, col_i16 x, col_i16 y, col_i16 cols, col_i16 rows, col_i16 viewer,
                              col_i16 flags);

void redraw_map_rect(Win *w, col_i16 tx, col_i16 ty, col_i16 tw, col_i16 th)
{
    register col_i16 x;
    register col_i16 y;
    RECT r;
    col_i16 pw;
    col_i16 ph;

    x = tx * g_view_tile_w + g_view_org_x + w->ox;
    y = ty * g_view_tile_h + g_view_org_y + w->oy;
    pw = tw * g_view_tile_w;
    ph = th * g_view_tile_h;
    draw_map_overview(w, 0, 0, (*g_map_width), g_map_h,
                  g_reveal_all != 0 ? -1 : g_view_nation, 0);
    SetRect(&r, x, y, x + pw, y + ph);
    copy_rect_to_screen_1060_631c(w, &r);
}
