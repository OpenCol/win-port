/* terrain_is_ocean: candidate for 1040:433e (108 bytes).  Verdict: RELOCATABLE.
 *
 * Low five bits of the plane-0 byte equal 0x19 or 0x1a (the two water ids); 0 off the map.
 *
 * RENAMED 2026-09-27 from ocean: CONST PROPOSAL; the current binding is wrong. terrain_is_ocean is on no Windows function today: no record, no file, and only prose in tools/mac_bind.py:67 and docs/reconstruction/next-session.md:103. Mac terrain_is_ocean(short, short), CODE 14 0x1a6e, 160 bytes, has 37 callers, and 433e's 43 callers include all 37 (ColonyMouseMap, ReportsShip, StatWin::Stats, allied_invasion, build_map, check_tut, ... water_access). Callees: on_map, map_get on both. Layout: Mac CODE 14 runs terrain_type, terrain_at, terrain_is_ocean; Windows 1040 runs terrain_type (42b5), terrain_at (42e8), 433e. Arity (short, short) against (int x, int y). Size 160 against 108. The Mac ocean(short, short) is a different function: a map-generation step in CODE 11 whose only caller is build_map and which calls buffer_get/buffer_put. It is 1020:047e (see above). This rename must come first in its chain.
 */

#include "colonization.h"

#include "terrain.h"

extern unsigned char map_get(int x, int y);

int terrain_is_ocean(int x, int y)
{
    int terrain;

    if (!on_map(x, y))
        return 0;
    terrain = map_get(x, y);
    terrain = (unsigned char)(terrain & MAP0_TERRAIN_MASK);
    return terrain == TERRAIN_OCEAN || terrain == TERRAIN_SEA_LANE;
}
