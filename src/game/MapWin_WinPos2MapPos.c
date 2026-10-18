/* PORT: a click beyond the map's last square, in a window larger than the map, is that last
   square: the view stops at the map's size (MapWin_ComputeViewParameters), and the square
   under the click would be off the map (OpenCol/win-port#7). */
/* MapWin_WinPos2MapPos: candidate for 1038:a0f2 (303 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_WinPos2MapPos
 *
 * Converts a window position to the map square under it (view origin at DGROUP:0x4c44/46
 * plus the tile index) and returns which ninth of the tile was hit, numbered clockwise from
 * 8 (top-left) through 1 (top), 2 (top-right), 3, 4, 5, 6, 7 (left), with 0 the centre.
 * MapWin_TileSize is defined here because the shipped call to it is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" int MapWin_TileSize(MapWin *w);

int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my)
{
    int third;
    int fx;
    int cx;
    int cy;
    int fy;
    int tile;

    tile = MapWin_TileSize(o);
    cx = x / tile;
    cy = y / tile;
    /* PORT: a click beyond the map, in a window larger than it, is its nearest square (OpenCol/win-port#7) */
    if (cx > g_view_w - 1)
        cx = g_view_w - 1;
    if (cy > g_view_h - 1)
        cy = g_view_h - 1;
    *mx = g_view_x0 + cx;
    *my = g_view_y0 + cy;
    third = tile / 3;
    fx = (x - cx * tile) / third;
    fy = (y - cy * tile) / third;
    if (fx == 0) {
        if (fy == 0)
            return 8;
        else if (fy == 1)
            return 7;
        else
            return 6;
    } else if (fx == 1) {
        if (fy == 0)
            return 1;
        else if (fy == 1)
            return 0;
        else
            return 5;
    } else {
        if (fy == 0)
            return 2;
        else if (fy == 1)
            return 3;
        else
            return 4;
    }
}
