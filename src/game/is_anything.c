/* is_anything: candidate for 1038:bf7c (97 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * is_city, falling back to is_unit; -1 when the index is out of range.
 *
 * RENAMED 2026-09-26 from square_nation_any: who holds (x, y): the settlement's nation, else the units' nation,
 * else -1. All six callers that keep the result call it `owner`.
 *
 * RENAMED 2026-09-26 from square_owner: the Mac build's is_anything(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_plan; caller combat_fight; caller find_beach; caller plan_move; callee is_city), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int is_city(int x, int y);
extern int is_unit(int x, int y);
int is_anything(int x, int y)
{
    int n;

    if (!map_index_in_range(x, y))
        return -1;
    n = is_city(x, y);
    if (n < 0)
        n = is_unit(x, y);
    return n;
}
