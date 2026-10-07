/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 39 of the imported file. */
/* PORT: this file's own declarations of get_window_rect_1060_61dd removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 38 of the imported file. */
/* MiniWin_Draw: candidate for 1060:08dd (143 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Redraws the whole map into a window: the window's own rectangle at +0x77e is refreshed and
 * installed as the port's clip, the map is drawn over the full 0x4c40 x 0x4c42 extent from
 * the origin, and the clip is applied.
 *
 * THE LAST ARGUMENT IS THE VIEWER. With reveal-all set (SEG20:0x77a4) it is -1 and otherwise
 * the player's nation (SEG20:0x7798), which is the same pair `DrawColoniesRegion` tests
 * before it consults the seen-bit plane. So -1 means "see everything" here as well.
 *
 * The rectangle at +0x77e is passed to both calls and is the window's OWN storage, not a
 * local: `get_window_rect_1060_61dd` fills it and `GRPort_SetClip` reads it back.
 *
 * `get_window_rect_1060_61dd` and `MiniWin_BlastMiniRegion` are -called -- the second by the
 * five-byte NOP form, so it is `extern` and the first a stub (rule 47).
 *
 * RENAMED 2026-09-26 from redraw_map_1060_08dd: the Mac build's MiniWin::Draw(void): this only -- the whole overview redrawn -- and it is what the window's installed repaint handler (MiniWin_MiniUpdate, from MiniWin_Init) calls: a repaint is a Draw. MiniUpdate(void) shares the width and stays unbound (the empty 1060:08c1 beside it has no caller to decide it). The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern GameState g_game;   /* SEG20:0x7782 */
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern void MiniWin_BlastMiniRegion(MiniWin *w, int x, int y, int cols, int rows, int viewer,
                              int flags);

/* same object: 4-byte `push cs; call ` */

void MiniWin_Draw(MiniWin *w)
{
    get_window_rect_1060_61dd((ColChild *)((MiniWin *)((ColChild *)w)), &w->clip);
    GRPort_SetClip((GRPort *)w, &w->clip);
    MiniWin_BlastMiniRegion(w, 0, 0, g_map_size.w, g_map_size.h,
                  g_game.reveal_all != 0 ? -1 : g_game.view_nation, 0);
    GRPort_ResetClip((GRPort *)w);
}
