/* simple_cash_flow: candidate for 1018:48e9 (90 bytes, 4 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Records that the current colony uses `use` of good `idx` this turn: its production from
 * g_prod (less an adjustment for good 14 when one is set) and its stock at +0x9a go to
 * analyze_cash_flow with it. RENAMED from simple_cash_flow.
 *
 * RENAMED 2026-09-26 from set_goods_consumption: the Mac build's simple_cash_flow(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the four functions between net_cargo_change and get_progress, which are already bound, paired in order; the arity agrees and the Mac body is 1.18x this one.
 */

#include "colonization.h"

#include "goods.h"
#include "records.h"

extern int g_shortfall[];   /* SEG21:0x1b4; [GOODS_ORE] is 0x1c0 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void analyze_cash_flow(int good, int prod, int use, int stock);
void simple_cash_flow(int idx, int use)
{
    int good = idx;
    int prod = g_prod[good];

    if (good == GOODS_TOOLS && g_shortfall[GOODS_ORE] != 0)
        prod -= g_shortfall[GOODS_ORE];
    analyze_cash_flow(good, prod, use, g_cur_colony_rec->stock[good]);
}
