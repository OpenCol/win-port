/* AboutWin_DoCredits: candidate for 1060:784f (160 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 160, the census, not the 189-byte gap. The 29 bytes between are padding and the
 * next function's exception descriptor -- the same over-statement `AboutBox` has,
 * and for the same reason: both are exception-frame functions and the descriptor of the next
 * one lives in the gap.
 *
 * Runs the About window: close whatever dialog is up, note the time in two fields, pump until
 * the flag at DGROUP:0x4e64 is set, then hide both windows and put the game palette back.
 *
 * `w->start = w->last = MSGetTicks_1060_87d0();` is a chained assignment through a CONVERSION: the
 * `SHL EAX,16 / SHRD EAX,EDX,16` pair is rule 48, a signed/unsigned `long` mismatch between
 * what the call returns and what the fields hold. Two stores, one EAX, one call.
 *
 * The chain runs `t1 = t2 = v` and not the other way round: Borland stores the RIGHTMOST
 * target first, so the field at +0xba0 is written before the one at +0xb9c. Spelled the other
 * way the two stores swap and the function is wrong at one byte, twice.
 *
 * The loop's test is at the BOTTOM with a jump into it, so the body never runs if the flag is
 * already set.
 *
 * `MSDoAllTasks_1060_86b6` is called THREE times with no arguments and its result ignored, once before
 * the loop (right after closing the dialog) and once after each hide -- a pump or a yield, and the shipped emits all three
 * rather than hoisting any of them.
 *
 * `CTable_Fill(SEG20:0x1ac, 10, 0xec, w->saved)` restores 236 entries from the window's
 * own saved copy at +0x5e0, which is what makes this the teardown half of a save.
 *
 * RENAMED 2026-09-26 from about_run: the Mac build's AboutWin::DoCredits(void): this only, running the About box's credits until it is dismissed; AboutWin_UpdateCredits is its tick. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void AboutWin_UpdateCredits(AboutWin *w);
struct logpal { char pad[1]; };
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

/* same object: 4-byte `push cs; call ` */
extern void show_window_1060_87b1(AboutWin *w);

extern void MSHideWindow_of_1060_87fe(void *o);

void AboutWin_DoCredits(AboutWin *w)
{
    show_window_1060_87b1(w);
    MSDoAllTasks_1060_86b6();
    w->start = w->last = MSGetTicks_1060_87d0();
    while (g_about_done == 0) {
        AboutWin_UpdateCredits(w);
        MSDoTask_1060_87e7();
    }
    MSHideWindow_of_1060_87fe((DialogWin *)((char *)w + 0x26));
    CTable_Fill(&g_game_palette, 10, 0xec, w->palette);
    MSDoAllTasks_1060_86b6();
    MSHideWindow_of_1060_87fe((DialogWin *)&w->win);
    MSDoAllTasks_1060_86b6();
}
