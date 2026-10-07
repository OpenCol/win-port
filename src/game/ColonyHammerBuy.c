/* ColonyHammerBuy: candidate for 1010:7369 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ColonyWin_Init's wrapper: ColonyWin_colony_hammer_buy on the colony window record (SEG20:0x25be).
 *
 * RENAMED 2026-09-26 from colony_window_7388: the button action ColonyWin_Init installs on its buy button
 * (+0x950): ColonyWin_colony_hammer_buy on the colony window with the id.
 *
 * RENAMED 2026-09-26 from colony_buy_clicked: the Mac build's ColonyWin::colony_hammer_buy(short) -- the Mac identifier really is spelled so: ColonyWin_Init installs it as the Buy button's action (+0x950), the button that buys the hammers of the current build, and it takes the one short.
 *
 * RENAMED 2026-09-27 from ColonyWin_colony_hammer_buy: 7369 is a 31-byte Windows-only -C thunk, colony_buy_build(&g_colony_window, code). ColonyWin_Init installs it as the Buy button's action (set_button_action at +0x950). It has no call-graph callers. The Mac method has no such wrapper: its only caller is KeyClick. Why the first binding went wrong: the method was bound by install slot (ColonyWin_Init's +0x950 action). On Windows that slot holds the thunk, not the method, and the thunk has no strings, which the witness reports as a one-sided contradiction. The new name is descriptive and follows its neighbour 1010:734b ColonyHammerChange, the identical 30-byte thunk for the +0x972 button. I chose ColonyHammerBuy over ColonyWin_colony_hammer_buy_clicked for two reasons: the thunk is not a method, and a name with ColonyWin_colony_hammer_buy as its prefix is a bulk-rename hazard (see the global-names-are-prefixes memory). The name is free under a case-insensitive check. This rename must come first in its chain.
 *
 * RENAMED 2026-09-27 from colony_hammer_buy_clicked: BEYOND THE PROPOSALS. It corrects c2ce60cde's premise. The 7369 record says 'The Mac method has no such wrapper: its only caller is KeyClick'. That is false, and it looked true only because tools/mac_callgraph.py does not read BSR.L. Mac ColonyHammerBuy(short), CODE 30 0x0618, 46 bytes, is LINK; move.w 8(a6),-(sp); pea g_colony_window (A5 $65e4, the same global as ColonyHammerChange); bsr.l colony_hammer_buy (61FF 0000001E -> 0x646); UNLK; RTS. 7369 (31 bytes) is ColonyWin_colony_hammer_buy(&g_colony_window, code). Arity: (short) against (int code). Layout: third in the four-function block on both, directly before colony_hammer_buy. Neither has a direct caller; both are installed as a button action. The body keeps ColonyWin_colony_hammer_buy, and the thunk takes the Mac thunk's own name. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;

extern void ColonyWin_colony_hammer_buy(ColonyWin *w, int a);

void ColonyHammerBuy(int code)
{
    ColonyWin_colony_hammer_buy((ColonyWin *)&g_colony_window, code);
}
