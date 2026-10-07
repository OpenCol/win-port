/* get_num_orders: candidate for 1028:c499 (56 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Either nibble of the packed byte at +2 of the current trade-route STOP: the high one
 * when the argument is non-zero, the low one otherwise.
 *
 * DGROUP:0x4c1c is the STOP, not the route. `set_route` computes it as
 * `g_cur_route_rec + 0x22 + n * 0xa`, which is one ten-byte stop of the route
 * `set_dest` selected; `orders_pointer` is the reconstruction that proves
 * it. This header said "the current trade route" until the naming audit caught it.
 *
 * RENAMED 2026-09-26 from route_stop_flag_nibble_get: how many goods the current stop unloads (kind 0, the low nibble of +2) or loads (kind 1,
 * the high nibble). ReportWin_TradeRoutes draws "Unload Cargo" at x+0xe1 and "Load Cargo" at x+0x1a9, then
 * ReportWin_TradeRoute1 draws count(0) goods from slots 0..5 at x 231 and count(1) goods from slots
 * 6..11 at x 431 -- under those headings. So kind 0 is UNLOAD and kind 1 is LOAD.
 *
 * RENAMED 2026-09-26 from route_stop_count: the Mac build's get_num_orders(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between ask_port and set_num_orders, which are already bound, paired in order; the arity agrees and the Mac body is 1.21x this one.
 */

#include "colonization.h"

#include "records.h"

int get_num_orders(int high)
{
    int r;
    if (high)
        r = g_cur_route_stop->rest[0] >> 4;
    else
        r = g_cur_route_stop->rest[0] & 0xf;
    return r;
}
