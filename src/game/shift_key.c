/* shift_key: candidate for 1040:7cd7 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Answers whether the quantity prompt should be put up, by asking the message pump whether the
 * 0x100 key state is held: `Screen_ShiftKeyPressed(&g_main_win)`. The record is pushed because
 * the callee takes an argument, and `Screen_ShiftKeyPressed` declares it `void *unused1` --
 * the wrapper ignores it and returns `MSKeyPressed(0x100)`.
 *
 * `ADD SP,0x4` after the call is the caller cleaning up, so the callee is __cdecl and the
 * pointer is its one argument.
 *
 * ITS CALLERS ARE WHAT NAME IT: every one passes the result straight into a goods transfer as
 * the "ask" argument -- `EuropeWin_AttemptCargoToEurope(w, u, goods, shift_key())`,
 * `ColonyWin_AttemptCargoToShip(..., shift_key())`, `EuropeWin_AttemptCargoFromEurope(...)`.
 *
 * RENAMED 2026-09-26 from should_ask_quantity: the Mac build's shift_key(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller EuropeMouseUnits; callee ShiftKeyPressed), the runner-up gets 1, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "classes.h"
extern MainWin g_main_win;   /* SEG20:0x5ca */
extern int Screen_ShiftKeyPressed(void *unused1);

int shift_key(void)
{
    return Screen_ShiftKeyPressed((Screen *)&g_main_win);
}
