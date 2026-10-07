/* get_unit_region: candidate for 1038:2f99 (68 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 68, not the census 63. The trailing 5 bytes are the dead second epilogue.
 *
 * The plane-2 nibble of unit u's own tile -- `land_region_at` at the unit's
 * coordinates. The twin of `colony_tile_nibble`, which asks the same question of
 * a colony; `orders_road` reads the same value as the unit tile's land region and passes it to
 * `find_close_village` as the region to match.
 *
 * RENAMED 2026-09-26 from unit_tile_nibble: the Mac build's get_unit_region(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller collect_indian_stats; caller collect_stats; callee land_region_at), the runner-up gets 2, the arity agrees (1), and the Mac body is 1.12x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "records.h"

extern int land_region_at(int x, int y);

int get_unit_region(register int u)
{
    return land_region_at(g_units[u].x, g_units[u].y);
}
