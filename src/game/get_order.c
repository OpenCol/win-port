/* get_order: candidate for 1028:c515 (83 bytes).  Verdict: RELOCATABLE.
 * symbol: get_order
 * name-source: sibling-build
 *
 * Nibble i of the current trade-route stop: the high half of the byte for an odd index,
 * the low half for an even one. The read half of the pair with `set_order`.
 *
 * Filed as `tribe_nibble_1028_c515`; see `orders_pointer` for why that was wrong.
 *
 * RENAMED 2026-09-26 from route_stop_nibble_get: goods slot i of the current stop, one nibble each: slots 0..5 are the goods it unloads, 6..11
 * the goods it loads. ReportWin_TradeRoutes draws "Unload Cargo" at x+0xe1 and "Load Cargo" at x+0x1a9, then
 * ReportWin_TradeRoute1 draws count(0) goods from slots 0..5 at x 231 and count(1) goods from slots
 * 6..11 at x 431 -- under those headings. So kind 0 is UNLOAD and kind 1 is LOAD.
 *
 * RENAMED 2026-09-26 from route_stop_goods: the Mac build's get_order(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between set_num_orders and new_dest, which are already bound, paired in order; the arity agrees and the Mac body is 0.82x this one.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern char * orders_pointer(int i);
int get_order(int slot)
{
    unsigned char v;

    v = *orders_pointer(slot);
    if (slot & 1)
        v = v >> 4;
    else
        v = v & 0xf;
    return v;
}
