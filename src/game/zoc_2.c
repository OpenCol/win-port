/* zoc_2: candidate for 1038:294e (220 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Looks around (x, y) for a tile of the same land/ocean kind owned (by
 * is_unit or is_city) by a nation other than `nation`;
 * the owner goes to DGROUP:0x4c36 (-1 when none) and the result says
 * whether one was found.
 *
 * RENAMED 2026-09-26 from find_adjacent_owner: the Mac build's zoc_2(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (callee is_city; callee is_unit), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int terrain_is_ocean(int x, int y);
extern int is_unit(int x, int y);
extern int is_city(int x, int y);

int zoc_2(int x, int y, int nation)
{
    register int o;
    register int i;
    int nx;
    int ny;
    int oc;

    g_foreign_owner = -1;
    oc = terrain_is_ocean(x, y);
    for (i = 0; g_foreign_owner < 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        o = is_unit(nx, ny);
        if (o >= 0 && o != nation && terrain_is_ocean(nx, ny) == oc)
            g_foreign_owner = o;
        o = is_city(nx, ny);
        if (o >= 0 && o != nation && terrain_is_ocean(nx, ny) == oc)
            g_foreign_owner = o;
    }
    return g_foreign_owner >= 0;
}
