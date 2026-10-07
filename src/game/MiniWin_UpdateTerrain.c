/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 56 of the imported file. */
/* MiniWin_UpdateTerrain: candidate for 1060:096c (196 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `MiniWin_Draw`'s partial: redraw the map and then copy back only the rectangle the
 * caller's tile range covers, rather than the whole window.
 *
 * The tile-to-pixel conversion is the readable part, and it is the same four words twice:
 * `x = tx * g_view_tile_w + g_view_org_x + w->left` and the same for y, then the extent
 * `tw * g_view_tile_w` by `th * g_view_tile_h` with NO origin -- a size, not a corner. The
 * window's own origin at +0x76a is added to the corner and not to the size, which is what
 * distinguishes the two uses of the same multiplier.
 *
 * The map is drawn in FULL first and only the copy back is clipped, so the saving is in the
 * blit and not in the drawing.
 *
 * The viewer argument is -1 with reveal-all set and the player's nation otherwise, exactly as
 * in `MiniWin_Draw`.
 *
 * `g_view_tile_*` and `g_view_org_*` are reached DS-relative where `g_map_size.w` and `g_map_size.h`
 * four hundred bytes below them need an explicit segment load -- one module, two declarations
 * (rule 9).
 *
 * RENAMED 2026-09-26 from redraw_map_rect: the Mac build's MiniWin::UpdateTerrain(short, short, short, short): this plus a tile rectangle (tx, ty, tw, th), redrawing the overview for that rectangle; it sits in the MiniWin module (1060:05e8..1060:15d0). Its sibling MiniWin_Draw is left unnamed: Draw(void) and MiniUpdate(void) share its signature.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` */
extern void copy_rect_to_screen_1060_631c(MiniWin *w, RECT *r);

extern void MiniWin_BlastMiniRegion(MiniWin *w, int x, int y, int cols, int rows, int viewer,
                              int flags);

void MiniWin_UpdateTerrain(MiniWin *w, int tx, int ty, int tw, int th)
{
    register int x;
    register int y;
    RECT r;
    int pw;
    int ph;

    x = tx * g_view_tile_w + g_view_org_x + w->rect.left;
    y = ty * g_view_tile_h + g_view_org_y + w->rect.top;
    pw = tw * g_view_tile_w;
    ph = th * g_view_tile_h;
    MiniWin_BlastMiniRegion(w, 0, 0, g_map_size.w, g_map_size.h,
                  g_game.reveal_all != 0 ? -1 : g_game.view_nation, 0);
    SetRect(&r, x, y, x + pw, y + ph);
    copy_rect_to_screen_1060_631c((MiniWin *)((GameWin *)w), &r);
}
