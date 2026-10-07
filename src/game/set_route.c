/* set_route: candidate for 1028:bce8 (47 bytes).  Verdict: RELOCATABLE.
 *
 * Makes stop i of the current trade route current: records the stop index and points the
 * current-stop pointer at that stop's entry, ten bytes apart in a table 0x22 into the current route record. Called from
 * route_create, route_edit, ReportWin_TradeRoute1 and perform_orders_route; the
 * Macintosh build has set_route(short) with the same arity.
 */

#include "colonization.h"

#include "records.h"

void set_route(int i)
{
    g_route_stop = i;
    g_cur_route_stop = &g_cur_route_rec->stops[g_route_stop];
}
