/* owner_of: candidate for 1038:bb13 (65 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The square's OCCUPYING NATION: the high nibble of the plane-2 byte at (x, y), with 15
 * read as -1 for "nobody".
 *
 * Named from its callers, which agree without exception: `is_city`,
 * `is_colony` and `is_hostile` all wrap it and return it as a nation,
 * and `move_unit`, `find_beach` and `StatWin_Stats` keep it as `tn`,
 * `tnation`, `n` and `nat`. The name is left as the nibble because `is_city` is
 * already taken by the wrapper.
 *
 * The LOW nibble of the same byte is the connected-region id (`continent_at`).
 *
 * RENAMED 2026-09-26 from map_plane2_high_nibble: the Mac build's owner_of(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller Stats; caller find_beach; caller indian_move; caller is_city; caller is_colony), the runner-up gets 5, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"
#include "nations.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern unsigned char continent_get(int x, int y);
char owner_of(int x, int y)
{
    int v = (continent_get(x, y) & 0xf0) >> 4;

    if (v == MAP2_NIBBLE_NONE)
        v = NATION_NONE;
    return v;
}
