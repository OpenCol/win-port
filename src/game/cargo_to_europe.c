/* cargo_to_europe: candidate for 1048:51d6 (145 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 145, not the census 136. The trailing 9 bytes are the dead second epilogue;
 * a `db 0` pad follows and then data, which is why the gap to the next census entry is 174.
 *
 * The sell side of `cargo_from_europe`, which sits directly below it (1048:516b, 107 bytes, ending where this begins). `unload_cargo` empties
 * the hold and leaves how much it took at DGROUP:0x4bd2; anything over `sell_max` goes straight
 * back into the unit, and what is left is sold at `europe_bid_price` and reported to
 * `traffic_to_europe`, the market's sell-side twin of `traffic_from_europe`.
 *
 * An empty slot returns whatever `money` happens to hold, because the early exit jumps past
 * the only assignment to it -- the shipped code loads `[bp-4]` at the shared exit without
 * ever having written it on that path. That is what the bytes say and it is left alone.
 *
 * RENAMED 2026-09-26 from europe_sell_goods: the Mac build's cargo_to_europe(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller foreign_country; callee stow_cargo; callee unload_cargo), the runner-up gets 2, and the arity agrees (3). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int unload_cargo(int u, int slot);
extern int stow_cargo(int unit, int good, int amount);

/* same object: 4-byte `push cs; call ` */
extern int europe_bid_price(int g);
extern void traffic_to_europe(int good, int n);

int cargo_to_europe(int unit, int slot, register int sell_max)
{
    int money;
    int amount;
    int good;

    good = unload_cargo(unit, slot);
    if (good < 0)
        goto done;
    amount = g_cargo_amount;
    if (amount > sell_max) {
        stow_cargo(unit, good, amount - sell_max);
        g_cargo_amount = sell_max;
    }
    money = europe_bid_price(good) * g_cargo_amount;
    traffic_to_europe(good, g_cargo_amount);
done:
    return money;
}
