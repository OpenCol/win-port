/* in_view: candidate for 1038:2a76 (161 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when one of the eight tiles around (x, y) is owned by `nation`, by
 * is_unit or is_colony; 0 off the map.
 *
 * RENAMED 2026-09-26 from nation_adjacent: the Mac build's in_view(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_fortress; caller move_unit; callee is_colony; callee is_unit; callee on_map), the runner-up gets 3, and the arity agrees (3).
 */

#include "colonization.h"

extern int is_unit(int x, int y);
extern int is_colony(int x, int y);

int in_view(int x, int y, int nation)
{
    register int r = 0;
    register int i;
    int nx;
    int ny;

    if (on_map(x, y) == 0)
        goto done;
    for (i = 0; r == 0 && i < 8; i++) {
        nx = g_dir_dx[i] + x;
        ny = g_dir_dy[i] + y;
        r = is_unit(nx, ny) == nation;
        r |= is_colony(nx, ny) == nation;
    }
done:
    return r;
}
