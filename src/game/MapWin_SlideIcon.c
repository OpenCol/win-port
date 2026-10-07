/* PORT: this file's own declarations of copy_rect_to_screen_1038_e38f removed: the header declares it from the definition (W1 step 3). */
/* MapWin_SlideIcon: candidate for 1038:aba2 (737 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_SlideIcon
 *
 * SLIDES A UNIT'S ICON between two squares of a map window -- the one the game actually uses, where
 * `animate_unit_move` is the unused eight-step version. Five callers.
 *
 * The map is scrolled to show the square being left, the bounding box of the two squares is redrawn
 * from the map and its units, and that box is saved into an offscreen port. Each step restores the box,
 * draws the icon at its new place -- half a tile to the right, as units hang centred over their square --
 * and copies just that box to the screen.
 *
 * THE STEP SIZE depends on bit 4 of the flags at SEG20:0x7785: set, the icon moves four pixels a step
 * over `tile / 4` steps; clear, two pixels over `tile / 2`. Either way the loop runs one step past the
 * count, which is what lands the icon on the destination square.
 *
 * The `other` argument is tested and its body is EMPTY -- the sibling at 1040:3a4a draws that unit in its
 * own square there, and this one does not.
 *
 * Spellings: `tile` is initialised at its declaration, before the port's constructor (rule 72); the
 * window's origin is the LEFT operand of the pixel sums; and the two starting squares are `register`
 * parameters.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
class DlgControl;

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern "C" int possibly_center(int x1, int y1, int x2, int y2, int mode);   /* PORT: C linkage, as defined (W2) */
extern void UpdateTerrainRegion(int x, int y, int w, int h);
extern void GenerateUnitsRegion(int x, int y, int w, int h);
extern void PieceDraw_noport(int u, int flags, int x, int y, int w, int h);
extern int GRPort_Init(GRPort *p, void *r);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int MapWin_TileSize(MapWin *w);


void MapWin_SlideIcon(MapWin *w, int u, int flags, int other, register int x1,
                          register int y1, int x2, int y2)
{
    int tile = MapWin_TileSize(w);
    GRPort port;
    RECT r1;
    RECT r2;
    int minx;
    int miny;
    int sx;
    int sy;
    int bw;
    int bh;
    int steps;
    int px;
    int py;
    int dxs;
    int dys;
    int ux;
    int uy;
    int i;
    int half;

    MSDoAllTasks();
    possibly_center(x1, y1, x1, y1, 1);
    minx = min_int_1038_e446(x1, x2);
    miny = min_int_1038_e446(y1, y2);
    bw = tile;
    sx = 1;
    if (x1 != x2) {
        bw = bw * 2;
        sx = sx * 2;
    }
    bh = tile;
    sy = 1;
    if (y1 != y2) {
        bh = bh * 2;
        sy = sy * 2;
    }
    UpdateTerrainRegion(minx, miny, sx, sy);
    GenerateUnitsRegion(minx, miny, sx, sy);
    if (other >= 0) {
    }
    px = w->rect.left + (minx - g_view_x0) * tile;
    py = w->rect.top + (miny - g_view_y0) * tile;
    if (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_FAST_PIECE_SLIDE) {
        dxs = (x2 - x1 > 0 ? 1 : (x2 - x1 < 0 ? -1 : 0)) << 2;
        dys = (y2 - y1 > 0 ? 1 : (y2 - y1 < 0 ? -1 : 0)) << 2;
        steps = tile >> 2;
    } else {
        dxs = (x2 - x1 > 0 ? 1 : (x2 - x1 < 0 ? -1 : 0)) * 2;
        dys = (y2 - y1 > 0 ? 1 : (y2 - y1 < 0 ? -1 : 0)) * 2;
        steps = tile >> 1;
    }
    ux = (x1 - g_view_x0) * tile;
    uy = (y1 - g_view_y0) * tile;
    half = tile >> 1;
    SetRect(&r1, 0, 0, bw, bh);
    SetRect(&r2, px, py, px + bw, py + bh);
    GRPort_Init(&port, &r1);
    GRPort_CopyToPort(w, &port, &r2, &r1);
    for (i = 0; steps + 1 > i; i++) {
        GRPort_CopyToPort(&port, w, &r1, &r2);
        PieceDraw_noport(u, flags, ux + half, uy, g_tile_w, g_icon_h);
        copy_rect_to_screen_1038_e38f(w, &r2);
        ux += dxs;
        uy += dys;
    }
}

