/* resource_at: candidate for 1038:c066 (301 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: resource_at
 *
 * Which scenery sprite, if any, a land tile carries: a hash of its position
 * (x/4*17 + seed + y/4*19 + 31 for the forest terrains 8..0x17, low nibble)
 * must equal (x&3)*4 + (y&3) or its xor with 10; then the terrain's entry
 * in the SEG27:0x2ca table (6 when 0), which plane-1 bit 0x04 (MAP1_DEPLETED) turns into 0
 * for entry 12 and -1 otherwise.  -1 off the map, on native land, or with
 * the seed at DGROUP:0x1ca6 zero.  Four helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from tile_decoration: the Mac build's resource_at(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller Stats; caller colony_plan; caller indian_move; caller orders_build; caller plan_move), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"

extern int g_stub_a;

extern int is_village(int x, int y);
extern unsigned char map_get(int x, int y);
extern unsigned char feature_get(int x, int y);

int resource_at(int x, int y)
{
    int r = -1;
    int hash;
    int h;
    int cls;
    int terrain;

    if (g_scenery_seed == 0)
        goto done;
    if (map_index_in_range(x, y) == 0)
        return -1;
    if (is_village(x, y) >= 0)
        goto done;
    terrain = map_get(x, y);
    terrain &= 0xff3f;
    cls = (terrain >= 8 && terrain < 0x10) || (terrain >= 0x10 && terrain < 0x18);
    hash = (x >> 2) * 17 + g_scenery_seed + (y >> 2) * 19 + cls * 31;
    hash = hash & 0xf;
    h = ((x & 3) << 2) + (y & 3);
    if (h != hash && (hash ^ 0xa) != h)
        goto done;
    r = g_terrain_bonus[terrain_at(x, y)];
    if (r == 0)
        r = 6;
    if (feature_get(x, y) & MAP1_DEPLETED) {
        if (r == 12)
            r = 0;
        else
            r = -1;
    }
done:
    return r;
}
