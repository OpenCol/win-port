/* is_city: candidate for 1038:bf0d (111 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The plane-2 high nibble at (x, y) when the square is on the map and plane-1 bit 1 is set
 * there; -1 otherwise or out of range.
 *
 * RENAMED 2026-09-26 from square_nation: the nation of the settlement on (x, y): the plane-2 high nibble,
 * read only when plane-1 has MAP1_SETTLEMENT (a colony or village) there.
 *
 * RENAMED 2026-09-26 from square_settlement_nation: the Mac build's is_city(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller colony_indian_table; caller colony_plan; caller combat_fight; caller indian_move; caller move_unit), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char feature_get(int x, int y);
extern char owner_of(int x, int y);
int is_city(int x, int y)
{
    int n = -1;

    if (!map_index_in_range(x, y))
        return -1;
    if (on_map(x, y) && (feature_get(x, y) & MAP1_SETTLEMENT))
        n = owner_of(x, y);
    return n;
}
