/* is_colony: candidate for 1038:be19 (122 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * is_city restricted to owners below 4 (a European nation); -1 otherwise or out of range.
 *
 * RENAMED 2026-09-26 from square_european_nation: the Mac build's is_colony(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller best_defender; caller combat_fight; caller move_unit; caller negotiate_europe; caller plan_move), the runner-up gets 5, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"
#include "nations.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char feature_get(int x, int y);
extern char owner_of(int x, int y);
int is_colony(int x, int y)
{
    int n = NATION_NONE;

    if (!map_index_in_range(x, y))
        return NATION_NONE;
    if (on_map(x, y) && (feature_get(x, y) & MAP1_SETTLEMENT)) {
        n = owner_of(x, y);
        if (n >= NATION_EUROPEAN_COUNT)
            n = NATION_NONE;
    }
    return n;
}
