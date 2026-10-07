/* find_close_village: candidate for 1038:05b8 (254 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: find_close_village
 *
 * The native village nearest to (x, y) by octile distance, restricted to
 * `nation` when >= 0 and to villages whose tile has plane-2 nibble `land`
 * when >= 0; the distance goes to DGROUP:0x4bc6 and the village is made
 * current (set_village, -called and defined in-file
 * as a stub).  -1 when none.
 *
 * RENAMED 2026-09-26 from find_nearest_village: the Mac build's find_close_village(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_indian_table; caller colony_plan; caller plan_move; caller try_to_set_job; callee land_region_at), the runner-up gets 2, and the arity agrees (4).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_stub_a;
extern int land_region_at(int x, int y);
extern int xy_dist(int dx, int dy);

extern void set_village(int v);

int find_close_village(int x, int y, int nation, int land)
{
    register int village;
    register int dist;
    int best = -1;
    int bestd = 9999;
    int nib;

    for (village = 0; village < g_game.settlement_count; village++) {
        if (nation >= 0 && g_settlements[village].owner != (unsigned char)nation)
            continue;
        if (land >= 0) {
            nib = land_region_at(g_settlements[village].x, g_settlements[village].y);
            if (nib != land)
                continue;
        }
        dist = xy_dist(x - g_settlements[village].x, y - g_settlements[village].y);
        if (dist <= bestd) {
            best = village;
            bestd = dist;
        }
    }
    g_nearest_dist = bestd;
    if (best >= 0)
        set_village(best);
    return best;
}
