/* map_dist: candidate for 1040:7b96 (102 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: map_dist
 *
 * The eight-way move distance of the delta between (x1, y1) and (x2, y2).  The
 * distance helper is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from octile_distance_xy: the Mac build's map_dist(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller indian_move; caller kill_strategy; caller plan_move; caller plan_strategy; caller special_kill), the runner-up gets 3, and the arity agrees (4).
 */

#include "colonization.h"

extern int g_stub_a;

extern int xy_dist(int dx, int dy);

int map_dist(int x1, int y1, int x2, int y2)
{
    int dx;
    int dy;

    dx = x1 - x2 > 0 ? x1 - x2 : ~(x1 - x2) + 1;
    dy = y1 - y2 > 0 ? y1 - y2 : ~(y1 - y2) + 1;
    return xy_dist(dx, dy);
}

