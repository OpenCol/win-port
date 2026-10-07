/* boycott: candidate for 1048:497c (36 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * True when the current nation's parliament has boycotted goods `g`: bit `g` of the boycott mask
 * at +0x20 of the 316-byte record the pointer at DGROUP:0x4cc4 selects. Sixteen goods, sixteen
 * bits, one int.
 *
 * THE ARGUMENT IS A GOODS INDEX, which is what named this. It was `current_nation_flag_test(bit)`
 * on a field called `flags`, a spelling that made it a sibling of `has_father` -- a
 * different function reading a different thing, the 0x13c-byte record as one long bit array from
 * +0. Every caller passes goods: `europe_label_cargo` passes `goods`, `EuropeWin_DrawMarket` loops
 * over the market rows, and the dock and market clicks pair it with `free_boycott`.
 * `raise_taxes` already called this same field `boycott`, and reads it as the mask to skip
 * when it picks what the colonists will dump.
 *
 * RENAMED 2026-09-26 from current_nation_boycotts: the Mac build's boycott(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DrawMarket; caller EuropeMouseUnits; caller europe_click_ship; caller europe_click_unit), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int boycott(int goods)
{
    return g_cur_nation_rec->boycott & (1 << goods);
}
