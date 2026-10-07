/* scan_for_one_terrain: candidate for 1018:6361 (99 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when (x, y) is on the map and its terrain id (low five bits of plane 0) lies in
 * lo..hi (unsigned compares).
 *
 * RENAMED 2026-09-26 from terrain_in_range: the Mac build's scan_for_one_terrain(short, short, unsigned char, unsigned char), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (callee map_get; callee on_map), the runner-up gets 1, and the arity agrees (4). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "terrain.h"

extern unsigned char map_get(int x, int y);

int scan_for_one_terrain(int x, int y, unsigned char lo, unsigned char hi)
{
    int r = 0;
    unsigned char terrain;

    if (!on_map(x, y))
        goto done;
    terrain = map_get(x, y) & MAP0_TERRAIN_MASK;
    if (terrain >= lo && terrain <= hi)
        r = 1;
done:
    return r;
}
