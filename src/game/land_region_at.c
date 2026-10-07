/* land_region_at: candidate for 1038:bbda (113 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The square's CONNECTED-REGION id for an on-map, non-ocean square; -1 otherwise or out of
 * range. See `continent_at` for what the nibble holds and what writes it.
 *
 * RENAMED 2026-09-26 from square_region_if_land: the Mac build's land_region_at(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DrawTitle; caller explore_around_square; callee on_map), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int terrain_is_ocean(int x, int y);
extern unsigned char continent_at(int x, int y);
int land_region_at(int x, int y)
{
    int n = -1;

    if (!map_index_in_range(x, y))
        return -1;
    if (on_map(x, y) && !terrain_is_ocean(x, y))
        n = continent_at(x, y);
    return n;
}
