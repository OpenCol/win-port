/* map_draw_tile_rect -- shipped at 1038:ae83.
 *
 * Ported by tools/port.py from matched/game/map_draw_tile_rect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_view_x0;                            DGROUP:0x4c44
 *   extern int far g_view_y0;                            DGROUP:0x4c46
 *   extern char far g_backdrop_port[];                   SEG28:0x0000
 *   extern int g_stub_a;                                 
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MapWin_TileSize(MapWin *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void wipe_rows(void *port, void *w, RECT *r, col_i16 x, col_i16 y, col_i16 a, col_i16 b, col_i16 c);



void map_draw_tile_rect(MapWin *w, col_i16 x, col_i16 y, col_i16 wd, col_i16 ht)
{
    register col_i16 ts;
    register col_i16 px;
    RECT r;
    col_i16 py;

    ts = MapWin_TileSize(w);
    px = (x - g_view_x0) * ts;
    py = (y - g_view_y0) * ts;
    SetRect(&r, px, py, wd * ts + px, ht * ts + py);
    px = w->ox + (x - g_view_x0) * ts;
    py = w->oy + (y - g_view_y0) * ts;
    wipe_rows(g_backdrop_port, w, &r, px, py, -1, 1, 3);
}
