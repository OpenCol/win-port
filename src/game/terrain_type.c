/* terrain_type: candidate for 1040:42b5 (51 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Terrain id from a map byte: bit 0x20 (MAP0_HILLY) set means hills or mountains -- 0x1b
 * (TERRAIN_MOUNTAINS) with bit 0x80, 0x1c (TERRAIN_HILLS) without; otherwise the low five bits.
 *
 * RENAMED 2026-09-26 from terrain_from_map_byte: the Mac build's terrain_type(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller compute_yield; caller terrain_at), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "terrain.h"

int terrain_type(int map_byte)
{
    int terrain;

    if (map_byte & MAP0_HILLY) {
        if (map_byte & MAP0_MOUNTAINS)
            terrain = TERRAIN_MOUNTAINS;
        else
            terrain = TERRAIN_HILLS;
    } else {
        terrain = (unsigned char)(map_byte & MAP0_TERRAIN_MASK);
    }
    return terrain;
}
