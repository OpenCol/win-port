/* zoc_4: candidate for 1038:27d4 (245 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Looks around (x, y) for a tile owned by a nation other than `nation`
 * (is_city, else is_unit) whose land/ocean kind matches:
 * from a claimed tile the kind is (x, y)'s own, otherwise the neighbour's.
 * The owner goes to DGROUP:0x4c36 (-1 when none); returns whether found.
 *
 * RENAMED 2026-09-26 from find_adjacent_owner2: the Mac build's zoc_4(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller foreign_unit; caller indian_move; caller move_unit; caller plan_move; callee is_city), the runner-up gets 3, and the arity agrees (3).
 */

#include "colonization.h"

#include "nations.h"

extern int terrain_is_ocean(int x, int y);
extern int is_city(int x, int y);
extern int is_unit(int x, int y);

int zoc_4(int x, int y, int nation)
{
    register int o;
    register int i;
    int nx;
    int ny;
    int oc;
    int kind;
    int in_settlement;

    oc = terrain_is_ocean(x, y);
    in_settlement = is_city(x, y) >= 0;
    g_foreign_owner = -1;
    for (i = 0; g_foreign_owner < 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        kind = in_settlement != 0 ? oc : terrain_is_ocean(nx, ny);
        o = is_city(nx, ny);
        if (o < 0)
            o = is_unit(nx, ny);
        else
            oc = kind;
        if (o >= 0 && o != nation && kind == oc)
            g_foreign_owner = o;
    }
    return g_foreign_owner >= 0;
}
