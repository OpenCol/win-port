/* factory_cash_flow: candidate for 1018:4943 (90 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: factory_cash_flow
 *
 * Passes simple_cash_flow the raw goods `raw_goods` and, as its use, the production of the
 * finished goods k (SEG21:0x122), cut to two thirds when the colony has more than two levels of
 * k's building (building_of_job): the one caller pairs ore/tools, sugar/rum, tobacco/cigars,
 * cotton/cloth and furs/coats.
 * The three helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from set_slot_from_table: the Mac build's factory_cash_flow(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the third of the four functions between net_cargo_change and get_progress, which are already bound, paired in order; the arity agrees and the Mac body is 1.09x this one. The call-graph vote had already put it first (3 vs 1).
 */

#include "colonization.h"

extern int g_stub_a;

extern int building_of_job(int job);
extern int building_level(int bit);
extern void simple_cash_flow(int slot, int use);

void factory_cash_flow(int raw_goods, int k)
{
    register int use;
    int building;

    use = g_prod[k];
    building = building_of_job(k);
    if (building_level(building) > 2)
        use = use * 2 / 3;
    simple_cash_flow(raw_goods, use);
}
