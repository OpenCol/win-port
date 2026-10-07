/* xy_dist: candidate for 1040:7b48 (78 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An eight-way move distance for a delta: the larger absolute component
 * plus half the smaller.
 *
 * RENAMED 2026-09-26 from octile_distance: the Mac build's xy_dist(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_plan; caller find_close_colony; caller indian_move; caller map_dist), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

int xy_dist(int dx, int dy)
{
    int r;

    dx = dx > 0 ? dx : ~dx + 1;
    dy = dy > 0 ? dy : ~dy + 1;
    if (dx > dy)
        r = (dy >> 1) + dx;
    else
        r = (dx >> 1) + dy;
    return r;
}
