/* animate_unit_move: candidate for 1040:3a4a (1116 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: animate_unit_move
 *
 * SLIDES A UNIT FROM ONE SQUARE TO THE NEXT in eight steps. Nothing in the image calls it.
 *
 * The map is first scrolled to show the square being left. The area redrawn is the bounding box of the
 * two squares -- one tile each way, doubled on an axis the move crosses -- and it is repainted from the
 * map and its units. With `other` given, that unit is drawn once in its own square first.
 *
 * The background under the slide is saved into a port the size of that box, and each step restores it,
 * draws the unit's icon one pixel further along (+/-1 per step on each axis the move crosses), and blits
 * the box to the screen. A backwards move starts a whole tile along, so the unit leaves from the 
 * edge.
 *
 * AFTERWARDS, only when `other` is given, the two squares are put back: `other` is redrawn only when the caller's flag 0x10 is set,
 * and the moving unit is redrawn in the square it started from -- the animation does not leave it at the
 * destination; the caller's own redraw does that.
 *
 * `steps` is set to 8 twice and a `long` local is zeroed and never read. The doublings are `* 2`, not
 * `x + x`; the three zeroed locals are initialised at their declarations, which is why their stores come
 * before the port's constructor (rule 72); and `MapWin_TileSize` is a call here where the module's
 * other helpers are .
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, MapWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "records.h"
   /* MapWin : ColChild, the base as its bytes: 0x788 */

/* Both are added to a map coordinate AFTER the view origin is taken off and BEFORE the
 * multiply by the tile size, so they are in tiles, not pixels: the viewport's top-left
 * inside the window. Nothing in the corpus writes them. */

extern int MapWin_TileSize(MapWin *w);
extern "C" int possibly_center(int x1, int y1, int x2, int y2, int mode);   /* PORT: C linkage, as defined (W2) */
extern int GRPort_Init(GRPort *p, void *r);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void PieceDraw_noport(int u, int flags, int x, int y, int w, int h);

extern "C" void UpdateTerrainRegion(int x, int y, int w, int h);
extern "C" void ShowMapRegion(int x, int y, int w, int h);
extern "C" void GenerateUnitsRegion(int x, int y, int w, int h);

void animate_unit_move(int u, int flags, int other, int x1, int y1, int x2, int y2)
{
    int dxs;
    int dys;
    int w;
    int h;
    int px;
    int py;
    int minx;
    int miny;
    int ox = 0;
    int oy = 0;
    int px2;
    int py2;
    int steps;
    int i;
    int sx;
    int sy;
    long spare = 0;
    RECT r2;
    RECT r1;
    int tile;
    GRPort port;
    register int x;
    register int y;

    tile = MapWin_TileSize(&g_map_window);
    MSDoAllTasks();
    possibly_center(x1, y1, x1, y1, 1);
    minx = min_int_1040_4483(x1, x2);
    miny = min_int_1040_4483(y1, y2);
    w = tile;
    sx = 1;
    if (x1 != x2) {
        w = w * 2;
        sx = sx * 2;
    }
    h = tile;
    sy = 1;
    if (y1 != y2) {
        h = h * 2;
        sy = sy * 2;
    }
    steps = 8;
    UpdateTerrainRegion(minx, miny, sx, sy);
    GenerateUnitsRegion(minx, miny, sx, sy);
    if (other >= 0) {
        ox = g_units[other].x;
        oy = g_units[other].y;
        UpdateTerrainRegion(ox, oy, 1, 1);
        px = (ox - g_view_x0 + g_view_offset_tx) * g_tile_w;
        py = (oy - g_view_y0 + g_view_offset_ty) * g_tile_h + 8;
        PieceDraw_noport(other, 0, px, py, g_tile_w, g_icon_h);
    }
    px = (minx - g_view_x0) * tile + 10;
    py = (miny - g_view_y0) * tile + 0x20;
    SetRect(&r1, 0, 0, w, h);
    CopyRect(&r2, &r1);
    OffsetRect(&r2, px, py);
    GRPort_Init(&port, &r1);
    GRPort_CopyToPort(&g_map_port, &port, &r2, &r1);
    dxs = x2 - x1 > 0 ? 1 : (x2 - x1 < 0 ? -1 : 0);
    dys = y2 - y1 > 0 ? 1 : (y2 - y1 < 0 ? -1 : 0);
    x = (dxs < 0 ? tile : 0) + px;
    y = (dys < 0 ? tile : 0) + py;
    steps = 8;
    for (i = 0; i < steps; i++) {
        GRPort_CopyToPort(&port, &g_map_port, &r1, &r2);
        PieceDraw_noport(u, flags, x, y, g_tile_w, g_icon_h);
        ShowMapRegion(px - 1, py - 1, w + 1, h + 1);
        x += dxs;
        y += dys;
    }
    if (other >= 0) {
        UpdateTerrainRegion(ox, oy, 1, 1);
        if (flags & 0x10) {
            px2 = (ox - g_view_x0 + g_view_offset_tx) * g_tile_w;
            py2 = (oy - g_view_y0 + g_view_offset_ty) * g_tile_h + 8;
            PieceDraw_noport(other, 0, px2, py2, g_tile_w, g_icon_h);
        }
        UpdateTerrainRegion(x1, y1, 1, 1);
        px2 = (x1 - g_view_x0 + g_view_offset_tx) * g_tile_w;
        py2 = (y1 - g_view_y0 + g_view_offset_ty) * g_tile_h + 8;
        PieceDraw_noport(u, flags, px2, py2, g_tile_w, g_icon_h);
        ShowMapRegion(px, py, w, h);
    }
}

