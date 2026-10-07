/* mappos_to_winpos: candidate for 1038:a221 (421 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: mappos_to_winpos
 * name-source: descriptive
 *
 * The inverse of `MapWin_WinPos2MapPos`: the window pixel position of map square (x, y), written
 * through the two pointers. The Macintosh symbol table has `MapWin::WinPos2MapPos` and no name
 * for this direction, so the name is descriptive.
 *
 * The square at +0x77a/+0x77c is drawn in the middle of the frame -- half of (frame size less a
 * tile) in from the frame's corner -- and every other square a tile per step away from it.
 *
 * THE MAP WRAPS EAST-WEST, and this is where the view chooses which copy of a column to draw:
 * when x is further from the centre column than x less the map width, it is moved one map to
 * the west; otherwise, when it is further than x plus the width, one map to the east. The
 * distances are `ABS(v) = v > 0 ? v : ~v + 1`, the macro `map_dist` also
 * uses -- `NOT AX / INC AX` for the negation, and every argument re-evaluated in each arm,
 * which is why `x - w->cx` is computed up to six times.
 *
 * `x` is a `register` parameter and keeps SI; `tile` is a plain local the allocator promotes to
 * DI after it (rule 53). The three helpers come through the 4-byte `PUSH CS` form and are stubs
 * here (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

/* same object: 4-byte `push cs; call ` */
extern "C" int MapWin_TileSize(MapWin *w);

void mappos_to_winpos(MapWin *w, register int x, int y, int *px, int *py)
{
    int tile;
    int half_w;
    int half_h;
    int dx;
    int dy;

    tile = MapWin_TileSize(w);
    half_w = (rect_width_1038_dfbd(&w->rect) - tile) / 2;
    half_h = (rect_height_1038_dfdc(&w->rect) - tile) / 2;
    if (ABS(x - w->cx) > ABS(x - w->cx - g_map_size.w))
        x -= g_map_size.w;
    else if (ABS(x - w->cx) > ABS(x - w->cx + g_map_size.w))
        x += g_map_size.w;
    dx = x - w->cx;
    dy = y - w->cy;
    *px = dx * tile + half_w;
    *py = dy * tile + half_h;
    *px += w->rect.left;
    *py += w->rect.top;
}
