/* set_dest: candidate for 1028:bcc2 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Selects trade route n: the index at DGROUP:0x4c20 and a pointer to its
 * 0x4a-byte record (SEG20:0xc804) at 0x4c18 -- the same shape as
 * `set_europe` and `set_colony`.
 *
 * This was filed as `set_tribe`, which is wrong: `set_tribe`
 * is a different function at 1038:0000 and this one has nothing to do with tribes.
 * All six callers are trade-route code -- `route_get`, `route_edit`,
 * `route_delete`, `route_create`, `orders_route`, and the SEG10 site that follows
 * it straight into `dest_get` -- and every one of them then reads the route's
 * name and its `sea` byte out of the pointer this sets. The SEG4 caller is the colony
 * teardown that walks the routes after `g_game.colony_count` is decremented.
 *
 * THREE SLOTS, NOT TWO. `set_dest` writes the record pointer at 0x4c18 and the
 * index at 0x4c20; 0x4c1c is a THIRD thing, the current STOP -- `set_route`
 * computes it as `g_route_base + index * 10 + 0x22`, a 10-byte entry inside the record.
 * Six files had called that stop pointer `g_cur_route`, which is the route itself.
 *
 * RENAMED 2026-09-26 from set_current_route: the Mac build's set_dest(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller delete_colony; caller perform_orders_route; caller route_create; caller route_edit), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

void set_dest(int n)
{
    g_cur_route = n;
    g_cur_route_rec = &g_routes[g_cur_route];
}
