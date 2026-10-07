/* DoFizzle: candidate for 1068:c1e9 (54 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Hands its seven arguments straight to `wipe_rows` with a delay of 0 -- the
 * shuffled-row wipe, which that function's own header already names this address as the caller
 * of.
 *
 * FIFTEEN CALL SITES AND TWO WRONG GUESSES. Every caller passes
 * `(port, win, rect, x, y, -1, N)` with N in {8, 10, 0x10, 0x1e, 0x20}: the congress hall
 * flashing a new father's seat, the woodcut, the opening scene over the whole 640x480, and
 * `port_draw_at`. `show_fathers` called it "the same highlighter the woodcut uses"
 * and that is what it looks like from outside -- but it is a transition, and the one instruction
 * that says so is the call below.
 *
 * RENAMED 2026-09-27 from wipe_rect: Mac DoFizzle(GRPort&, GameWin&, Rect*, short, short, short, short), the 7-argument overload. Callers: Opening::DoShip, AboutWin::UpdateCredits, show_fathers, show_ship and woodcut_show, 5 of 5 (Windows adds port_draw_at). Callee: the 8-argument DoFizzle, which is wipe_rows here, on both. Layout: Mac LowMemTimerProc, EnableLowMemoryCheck, DisableLowMemoryCheck, DoFizzle(7), DoFizzle(8), FrameInfoRect; Windows EnableLowMemoryCheck, DisableLowMemoryCheck, gif_to_pic (Windows-only), c1e9, wipe_rows, draw_bevel. Arity 7. wipe_rows is the overload twin and keeps its name.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void wipe_rows(GameWin *win_a, GameWin *win_b, RECT *src_rect, int x, int y, int mode, int step, int delay);

void DoFizzle(GRPort *port, GameWin *win, RECT *r,
                             int x, int y, int mode, int step)
{
    wipe_rows((GameWin *)port, (GameWin *)win, r, x, y, mode, step, 0);
}
