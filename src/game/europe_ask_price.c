/* europe_ask_price: candidate for 1048:406e (74 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The market's price byte for good g plus the good's buy spread, floored at 0.
 *
 * SEG20:0xd7a0 IS NOT A TABLE OF ITS OWN: it is `g_goods` (0xd79c) + 4, member +4 of the
 * 10-byte record `load_data` fills from the CARGO resource, nine numbers per good.
 * `europe_adjust_prices` names +2, +3, +5, +6 and +7 `price_min`, `price_max`, `up`, `down`
 * and `drift` from the same record, and declares +4 `spread` without reading it; this function is its only
 * reader and so gets to name it. CARGO's fifth number is 7 for Food, 1 for the four crops,
 * 4 for Lumber, 2 for Ore and 0 for silver and everything manufactured -- a spread the raw
 * goods carry and the finished ones do not, which is what the name says.
 *
 * RENAMED 2026-09-26 from goods_price_buy: the Mac build's europe_ask_price(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller DrawMarket; caller ReportsEcon; caller europe_click_unit; caller foreign_country; caller traffic_from_europe), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int europe_ask_price(int goods)
{
    int p;

    p = g_cur_nation_rec->price[goods];
    p = g_goods[goods].spread + p;
    p = max_int_1048_dd61(0, p);
    return p;
}
