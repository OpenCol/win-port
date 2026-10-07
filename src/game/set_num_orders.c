/* set_num_orders: candidate for 1028:c4d1 (68 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Writes one nibble of the current trade-route STOP's packed byte at +2 (the stop pointer
 * is at DGROUP:0x4c1c): the high nibble when `high` is set, the low one otherwise.
 *
 * 0x4c1c is the STOP and not the route -- see `orders_pointer`, which proves it
 * from `set_route`'s arithmetic. This header said "route" until the audit.
 *
 * RENAMED 2026-09-26 from route_stop_flag_nibble_set: sets how many goods the current stop unloads (kind 0, low nibble of +2) or loads (kind 1,
 * high nibble). ReportWin_TradeRoutes draws "Unload Cargo" at x+0xe1 and "Load Cargo" at x+0x1a9, then
 * ReportWin_TradeRoute1 draws count(0) goods from slots 0..5 at x 231 and count(1) goods from slots
 * 6..11 at x 431 -- under those headings. So kind 0 is UNLOAD and kind 1 is LOAD.
 *
 * RENAMED 2026-09-26 from route_stop_count_set: the Mac build's set_num_orders(short, short), bound by call-graph agreement: 1 of its Mac call edges already bound land on this function (caller new_dest), the runner-up gets 0; and by call-sequence alignment: in new_dest the calls either side align and this is the one unbound callee left in that gap (1.0 votes, next 0.0); the arity agrees (2), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "records.h"

void set_num_orders(int high, int count)
{
    if (high != 0) {
        g_cur_route_stop->rest[0] &= 0xf;
        g_cur_route_stop->rest[0] |= count << 4;
    } else {
        g_cur_route_stop->rest[0] &= 0xf0;
        g_cur_route_stop->rest[0] |= count & 0xf;
    }
}
