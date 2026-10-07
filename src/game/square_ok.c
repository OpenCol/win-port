/* square_ok: candidate for 1030:308a (131 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether unit u may be put down on (x, y): the square must be on the map, of terrain 0x1a, and
 * either free of units or holding units of u's own nation (is_unit's result against the low nibble of Unit+3). The ring search in
 * stack_to_board takes the first square this accepts.
 *
 * RENAMED 2026-09-26 from square_accepts_unit: the Mac build's square_ok(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (callee is_unit; callee on_map), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "terrain.h"
#include "records.h"

extern int is_unit(int x, int y);

int square_ok(int u, int x, int y)
{
    int ok;
    int n;

    ok = 0;
    if (on_map(x, y) == 0)
        goto out;
    if (terrain_at(x, y) != TERRAIN_SEA_LANE)
        goto out;
    n = is_unit(x, y);
    if (n >= 0 && (unsigned char)(g_units[u].flags & 0xf) != n)
        goto out;
    ok = 1;
out:
    return ok;
}
