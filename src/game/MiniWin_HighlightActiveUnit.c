/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 39 of the imported file. */
/* PORT: this file's own declarations of get_window_rect_1060_61dd removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 38 of the imported file. */
/* MiniWin_HighlightActiveUnit: candidate for 1060:0fdb (146 bytes).  Verdict: RELOCATABLE.
 * symbol: MiniWin_HighlightActiveUnit
 * name-source: sibling-build
 *
 * Repaints the active unit's cell in the whole-map overview.  The caller toggles
 * `blink` every 25 ticks.  This routine limits drawing to the overview rectangle,
 * asks `MiniWin_HighlightActiveUnit_cell` to restore or flash the cell, and then
 * applies the port's wanted rectangle again.
 *
 * Reveal-all mode supplies viewer -1; otherwise the current viewing nation is
 * used.  The two zeroes cover the overview from map origin (0,0), and the global
 * map dimensions make the draw cover every possible active-unit cell.
 *
 * RENAMED 2026-09-26 from blink_overview_unit: the Mac build's MiniWin::HighlightActiveUnit(char): this plus one flag, repainting the active unit's cell of the overview -- the only function of the MiniWin module (1060:05e8..1060:15d0) about the active unit apart from its own worker.
 * Re-checked 2026-09-28: the long form of the Mac table adds a second overload, MiniWin::HighlightActiveUnit(short x6, char), 18 bytes with this. That one is MiniWin_HighlightActiveUnit_cell (1060:106d, 18 bytes, called only from here), not this function: this one cleans 6 at both sites, which is the (char) overload, and its Mac caller is GameBrains::DoHuman, as here. The binding stands.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern void MiniWin_HighlightActiveUnit_cell(MiniWin *w, int x,
                                                 int y, int cols, int rows,
                                                 int viewer, int white,
                                                 char blink);

/* same object: 4-byte `push cs; call ` (rule 47) */

void MiniWin_HighlightActiveUnit(MiniWin *w, char blink)
{
    get_window_rect_1060_61dd((ColChild *)((MiniWin *)((ColChild *)w)), &w->clip);
    GRPort_SetClip((GRPort *)w, &w->clip);
    MiniWin_HighlightActiveUnit_cell(w, 0, 0, g_map_size.w, g_map_size.h,
                                    g_game.reveal_all != 0 ? -1 : g_game.view_nation,
                                    0, blink);
    GRPort_ResetClip((GRPort *)w);
}
