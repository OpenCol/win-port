/* lost_city_at: candidate for 1038:c193 (196 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: lost_city_at
 *
 * A pseudo-random per-tile test used for scenery: on a land tile that is
 * not terrain 24..26, with no high plane-2 nibble, hashes the tile's
 * position (x/4*17 + seed + y/4*19 + 8, low five bits) against
 * (x&3)*4 + (y&3); 0 when the seed at DGROUP:0x1ca6 is 0 or the tile is
 * outside the map.  The two helpers are -called and defined in-file
 * as stubs.
 *
 * RENAMED 2026-09-26 from tile_is_river_mouth: the Mac build's lost_city_at(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller Stats; caller close_move; caller colony_field_status; caller indian_move; caller move_unit), the runner-up gets 3, and the arity agrees (2). The old name was a guess at what the per-tile position hash marks; a pseudo-random, seeded, one-in-N land-tile test is how the game scatters Lost City Rumours.
 */

#include "colonization.h"

#include "terrain.h"

extern int g_stub_a;

extern char owner_of(int x, int y);

int lost_city_at(int x, int y)
{
    int r = 0;
    int hash;
    int hi;
    int terrain;

    if (map_index_in_range(x, y) == 0)
        return 0;
    if (g_scenery_seed == 0)
        goto done;
    terrain = terrain_at(x, y);
    if (terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE)
        goto done;
    if (terrain == TERRAIN_ARCTIC)
        goto done;
    hi = owner_of(x, y);
    if (hi >= 0)
        goto done;
    hash = (x >> 2) * 17 + g_scenery_seed + (y >> 2) * 19 + 8;
    if (((x & 3) << 2) + (y & 3) != (hash & 0x1f))
        goto done;
    r = 1;
done:
    return r;
}
