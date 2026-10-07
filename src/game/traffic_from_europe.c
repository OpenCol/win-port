/* traffic_from_europe: candidate for 1048:4fbe (176 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The bookkeeping behind buying `amount` of `good` in Europe. The caller has already taken
 * the gold -- every one of the five does, either through `subtract_gold` or by
 * subtracting from the nation record's own +0x2a -- and this moves the goods.
 *
 * All FOUR European nations lose volume, not just the buyer: `volume[good]` at +0x5c of
 * each 316-byte nation record drops by `cargo_diff_adjust(amount) + (amount << shift)`, where the
 * shift is +8 of the 10-byte per-good record at SEG20:0xd79c. That is the price mechanism
 * -- one player's purchase moves every market, and `market_price_rise` is what turns an
 * exhausted accumulator into a higher price.
 *
 * The buyer's own record then loses the amount from two long-per-good running totals at
 * +0xbc and +0xfc, and `amount * europe_ask_price(good)` from a third at +0x7c. Those three
 * arrays are the tail of the record: 0x7c + 64 = 0xbc, + 64 = 0xfc, + 64 = 0x13c, which is
 * its size exactly.
 *
 * `g_cur_nation_rec` and `g_nations` are the same array -- the pointer at DGROUP:0x4cc4 is a nation
 * record, and the "market" the price functions read is the nation's own copy of it.
 *
 * RENAMED 2026-09-26 from market_record_purchase: the Mac build's traffic_from_europe(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller colony_plan; caller europe_click_unit; caller foreign_country), the runner-up gets 2, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "nations.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

/* same object: 4-byte `push cs; call ` */
extern int cargo_diff_adjust(int amount);
extern int europe_ask_price(int g);

void traffic_from_europe(register int good, int amount)
{
    int price;
    int total;
    register int n;

    for (n = 0; n < NATION_EUROPEAN_COUNT; n++)
        g_nations[n].drift[good] -= cargo_diff_adjust(amount)
                                   + (amount << g_goods[good].shift);
    g_cur_nation_rec->trade_tons[good] -= amount;
    g_cur_nation_rec->sold[good] -= amount;
    price = europe_ask_price(good);
    total = amount * price;
    g_cur_nation_rec->trade_gold[good] -= total;
}
