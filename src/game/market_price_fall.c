/* market_price_fall: candidate for 1048:4f20 (70 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The European price of good g goes down one, floored at 0 through `max_int`. The twin
 * of `market_price_rise`, and the same floor `europe_bid_price` applies when it reports
 * the sell price.
 *
 * `p--` and not `p = p - 1`: the shipped code decrements the slot in place with a single
 * `DEC WORD [bp-4]`, where the load-subtract-store spelling costs four bytes more. `p`
 * has a stack slot here only because it has to survive the call to `max_int`; in the
 * twin, which has no call, the same variable lives in DX and the frame stays empty.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void market_price_fall(register int goods)
{
    int p;

    p = g_cur_nation_rec->price[goods];
    p--;
    p = max_int_1048_dd61(0, p);
    g_cur_nation_rec->price[goods] = (signed char)p;
}
