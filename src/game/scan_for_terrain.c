/* scan_for_terrain: candidate for 1018:63c4 (83 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: scan_for_terrain
 *
 * Sums scan_for_one_terrain(x + dx, y + dy, a, b) over the eight neighbours.
 * scan_for_one_terrain is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from count_terrain_around: the Mac build's scan_for_terrain(short, short, unsigned char, unsigned char), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller compute_yield; callee scan_for_one_terrain), the runner-up gets 1, the arity agrees (4), and the Mac body is 1.42x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

extern int g_stub_a;

extern int scan_for_one_terrain(int x, int y, unsigned char a, unsigned char b);

int scan_for_terrain(int x, int y, unsigned char lo, unsigned char hi)
{
    register int n = 0;
    register int dir;

    for (dir = 0; dir < 8; dir++)
        n += scan_for_one_terrain(g_dir_dx[dir] + x, g_dir_dy[dir] + y, lo, hi);
    return n;
}
