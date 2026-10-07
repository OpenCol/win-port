/* cargo_from_europe: candidate for 1048:516b (107 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 107, not the census 98. The trailing 9 bytes are a second epilogue.
 *
 * One European purchase, start to finish: at most a hundred of `good`, priced by
 * `europe_ask_price`, the gold taken from the current nation, the market moved by
 * `traffic_from_europe`, and the goods put in the unit's hold. Returns what it cost.
 *
 * The cap is `min_int(want, 100)` -- a full cargo hold and no more per transaction.
 *
 * RENAMED 2026-09-26 from europe_buy_goods: the Mac build's cargo_from_europe(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller foreign_country; callee stow_cargo; callee traffic_from_europe), the runner-up gets 1, and the arity agrees (3).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void subtract_gold(int nation, long amount);
extern int stow_cargo(int unit, int good, int amount);

/* same object: 4-byte `push cs; call ` */
extern int europe_ask_price(int g);
extern void traffic_from_europe(int good, int n);

int cargo_from_europe(int unit, int good, int want)
{
    int amount;
    int cost;

    amount = min_int_1048_db44(want, 100);
    cost = amount * europe_ask_price(good);
    subtract_gold(g_cur_nation, (long)cost);
    traffic_from_europe(good, amount);
    stow_cargo(unit, good, amount);
    return cost;
}
