/* map_draw_tile_rect: candidate for 1038:ae83 (199 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: map_draw_tile_rect
 *
 * Draws the w x h tile block at map (x, y) from the backdrop port (SEG28:0)
 * through wipe_rows: the source rectangle in tile units times the tile
 * size, the destination at the window origin plus the same offset.
 * MapWin_TileSize is -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;

#include "records.h"
extern void wipe_rows(GameWin *win_a, GameWin *win_b, RECT *src_rect, int x, int y, int mode, int step, int delay);

extern "C" int MapWin_TileSize(MapWin *w);

void map_draw_tile_rect(MapWin *w, int x, int y, int wd, int ht)
{
    register int ts;
    register int px;
    RECT r;
    int py;

    ts = MapWin_TileSize(w);
    px = (x - g_view_x0) * ts;
    py = (y - g_view_y0) * ts;
    SetRect(&r, px, py, wd * ts + px, ht * ts + py);
    px = w->rect.left + (x - g_view_x0) * ts;
    py = w->rect.top + (y - g_view_y0) * ts;
    wipe_rows((GameWin *)((MSWin *)&g_map_port), (GameWin *)((MSWin *)w), &r, px, py, -1, 1, 3);
}
