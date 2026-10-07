/* europe_bid_price: candidate for 1048:40b8 (54 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The market's price byte for good g (+0x4c of the current nation's record, pointed to from DGROUP:0x4cc4) minus one,
 * floored at 0.
 *
 * RENAMED 2026-09-26 from goods_price_sell: the Mac build's europe_bid_price(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller DrawMarket; caller ReportsEcon; caller europe_click_unit; caller foreign_country; caller new_turn_colony), the runner-up gets 4, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int europe_bid_price(int goods)
{
    int p;

    p = g_cur_nation_rec->price[goods];
    p = p - 1;
    p = max_int_1048_dd61(0, p);
    return p;
}
