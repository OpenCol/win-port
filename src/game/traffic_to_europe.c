/* traffic_to_europe: candidate for 1048:506e (253 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The mirror of `traffic_from_europe` eighty bytes above it: every sign is flipped.
 * All four European nations gain `volume[good]` at +0x5c instead of losing it, the buyer's
 * two long-per-good totals at +0xbc and +0xfc gain the amount, and the third at +0x7c gains
 * the proceeds.
 *
 * TWO THINGS THE SELL SIDE HAS THAT THE BUY SIDE DOES NOT.
 *
 * Nation 3 gets two thirds. `n == 3` takes `(base * 2) / 3` where the other three take
 * `base` -- a signed `IDIV` by an immediate 3, so it really is that expression and not a
 * shift. Nothing in the buy path does anything of the kind.
 *
 * It is a TERNARY, not an `if`/`else`. Both spell the same branch, but the ternary stores
 * once at the join and the `if`/`else` stores in each arm: the shipped `EB 03` jumps to the
 * single `MOV [bp-6],AX`, where two assignments put a `MOV` before the jump and make it
 * `EB 06`.
 *
 * And the proceeds are taxed: `(long)gross * (100L - tax) / 100`, with the tax rate a
 * signed byte at +1 of the nation record -- the same byte `king_adjust_tax` moves. The
 * arithmetic is done in 32 bits: `CBW`, `CWDE`, `MOV EDX,100`, `SUB EDX,EAX`, `IMUL
 * EAX,EDX`, `CDQ`, `IDIV EBX`, which is what `-3` buys and what makes this function a
 * hundred bytes longer than its twin.
 *
 * THE `L` IS ON THE HUNDRED AND NOWHERE ELSE. `100L - tax` promotes the tax to 32 bits and
 * subtracts there, which is the shipped `CWDE` before `MOV EDX,100`. `(long)(100 - tax)`
 * subtracts in sixteen bits and widens afterwards -- a different instruction at the same
 * offset -- and `(long)tax` moves the divergence six bytes earlier still, because it
 * changes how the byte is loaded. Three spellings of one arithmetic identity, one of which
 * is the source.
 *
 * RENAMED 2026-09-26 from market_record_sale: the Mac build's traffic_to_europe(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller europe_click_unit; caller foreign_country; caller new_turn_colony; caller plan_move), the runner-up gets 3, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "nations.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

/* same object: 4-byte `push cs; call ` */
extern int cargo_diff_adjust(int amount);
extern int europe_bid_price(int g);

void traffic_to_europe(register int good, int amount)
{
    int base;
    int add;
    int price;
    int gross;
    long net;
    register int n;

    base = cargo_diff_adjust(amount) + (amount << g_goods[good].shift);
    for (n = 0; n < NATION_EUROPEAN_COUNT; n++) {
        add = (n == 3) ? base * 2 / 3 : base;
        g_nations[n].drift[good] += add;
    }
    g_cur_nation_rec->trade_tons[good] += amount;
    g_cur_nation_rec->sold[good] += amount;
    price = europe_bid_price(good);
    gross = price * amount;
    net = (long)gross * (100L - g_cur_nation_rec->tax) / 100;
    g_cur_nation_rec->trade_gold[good] += net;
}
