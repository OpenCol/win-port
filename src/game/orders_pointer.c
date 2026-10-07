/* orders_pointer: candidate for 1028:c43d (92 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A pointer to the byte holding nibble i of the current trade-route STOP. The stop record
 * is the pointer at DGROUP:0x4c1c, which `set_route` computes as
 * `g_cur_route_rec + 0x22 + n * 0xa` -- so this reaches into one 10-byte stop of the
 * route selected by `set_dest`.
 *
 * Twelve nibbles in six bytes, in two groups of six: 0..5 are packed two to a byte from
 * +6 and 6..11 from +3. A ship has six cargo holds, and the pair of six-slot lists is
 * what a trade-route stop needs -- what to load here and what to unload -- but nothing in
 * these three functions says which group is which. ReportWin_TradeRoute1 does: slots 0..5 sit
 * under "Unload Cargo", 6..11 under "Load Cargo" (see get_order).
 *
 * `SUB CX,6` on the parameter itself and a bare `SAR` say the source subtracts in place
 * and shifts rather than dividing: `/ 2` would carry the sign correction.
 *
 * This was filed as `tribe_field_ptr_1028_c43d`. DGROUP:0x4c1c is the current trade-route
 * stop -- four other reconstructions already call it `g_cur_route_stop` -- and both callers,
 * `get_order` and `set_order`, are route code. See `set_dest`,
 * which was misnamed the same way for the same reason.
 *
 * RENAMED 2026-09-26 from route_stop_nibble_ptr: the Mac build's orders_pointer(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between ask_port and set_num_orders, which are already bound, paired in order; the arity agrees and the Mac body is 0.78x this one.
 */

#include "colonization.h"

#include "records.h"

char *orders_pointer(int i)
{
    char *p;

    if (i >= 6) {
        i -= 6;
        p = (char *)g_cur_route_stop + (i >> 1) + 3;
    } else {
        p = (char *)g_cur_route_stop + (i >> 1) + 6;
    }
    return p;
}
