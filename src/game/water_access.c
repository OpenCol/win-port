/* water_access: candidate for 1018:39e3 (187 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Looks at the eight neighbours of (x, y) for ocean tiles and remembers, in
 * DGROUP:0x4bc8/0x4bca, the one with the lowest plane-2 nibble (0 reads as
 * 16).  Returns 1 when any neighbour is ocean.
 *
 * RENAMED 2026-09-26 from find_adjacent_ocean: the Mac build's water_access(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller create_colony; caller plan_move; caller route_create; callee on_map), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

extern int terrain_is_ocean(int x, int y);
extern unsigned char continent_at(int x, int y);

int water_access(int x, int y)
{
    register int dir;
    register int nx;
    int found = 0;
    int ny;
    int continent;
    int best = -1;

    for (dir = 0; dir < 8; dir++) {
        nx = g_dir_dx[dir] + x;
        ny = g_dir_dy[dir] + y;
        if (on_map(nx, ny) != 0 && terrain_is_ocean(nx, ny) != 0) {
            found = 1;
            continent = continent_at(nx, ny);
            if (continent == 0)
                continent = 0x10;
            if (best < 0 || continent < best) {
                best = continent;
                g_ocean_x = nx;
                g_ocean_y = ny;
            }
        }
    }
    return found;
}
