/* analyze_cash_flow: candidate for 1018:4869 (128 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Records how much of one good a colony USES this turn, and what that leaves uncovered: the
 * consumption into `g_consumption`, how it exceeds this turn's production into `g_deficit`
 * (drawn from stock), and how it exceeds production AND stock together into `g_shortfall`
 * (unmet). Both are cleared first and written only when the comparison says so.
 *
 * RENAMED from analyze_cash_flow, with its tables (g_value, g_over_lo, g_over_band). The
 * caller settles what the arguments are: simple_cash_flow passes g_prod[good] as the
 * second and the colony's stock as the fourth, and compute_colony_yield passes what the
 * colonists EAT for food, and the hammer and musket output for the lumber and tools they burn.
 * The readers agree: `g_prod[i] - g_consumption[i]` is the net change, and colony_plan
 * asks whether `g_deficit[FOOD] << 4` still fits in the stock -- sixteen turns of it.
 *
 * Three `int` tables 0x28 bytes apart, each indexed by `i` doubled: twenty entries apiece.
 * The index is recomputed for every store, five times over, because nothing survives in a
 * register between them.
 *
 * The second test is written the way the shipped code reads it -- `lo + span < v`, with the
 * sum on the LEFT. Written `v > lo + span` Borland loads `v` first and the compare comes out
 * the other way round.
 *
 * RENAMED 2026-09-26 from set_consumption: the Mac build's analyze_cash_flow(short, short, short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the four functions between net_cargo_change and get_progress, which are already bound, paired in order; the arity agrees and the Mac body is 1.05x this one.
 */

#include "colonization.h"

extern int g_consumption[];      /* SEG21:0x164 */
extern int g_deficit[];    /* SEG21:0x18c */
extern int g_shortfall[];  /* SEG21:0x1b4 */

void analyze_cash_flow(int good, int prod, int use, int stock)
{
    g_consumption[good] = use;
    g_deficit[good] = 0;
    g_shortfall[good] = 0;
    if (use > prod)
        g_deficit[good] = use - prod;
    if (prod + stock < use)
        g_shortfall[good] = use - (prod + stock);
}
