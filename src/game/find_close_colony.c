/* find_close_colony: candidate for 1018:3a9e (312 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: find_close_colony
 *
 * The colony nearest to (x, y) by xy_dist's distance, restricted to
 * `nation` when that is >= 0 and to colonies whose tile has plane-2 nibble
 * `land` when that is >= 0; land == -2 means "the same as (x, y)" and also
 * requires flag 0x40 in Colony+0x1c.  The distance goes to DGROUP:0x4bc6
 * and the colony is made current.  set_colony is -called and
 * defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from find_nearest_colony: the Mac build's find_close_colony(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller combat_fight; caller plan_move; callee land_region_at; callee set_colony), the runner-up gets 2, and the arity agrees (4).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_stub_a;
extern int land_region_at(int x, int y);
extern int xy_dist(int dx, int dy);

extern void set_colony(int colony);

int find_close_colony(int x, int y, int nation, int land)
{
    register int colony;
    int dist;
    int best = -1;
    int bestd = 9999;
    int nib;
    int check = 0;

    if (land == -2) {
        check = 1;
        land = land_region_at(x, y);
    }
    for (colony = 0; colony < g_game.colony_count; colony++) {
        if (nation >= 0 && g_colonies[colony].nation != (unsigned char)nation)
            continue;
        if (land >= 0) {
            nib = land_region_at(g_colonies[colony].x, g_colonies[colony].y);
            if (nib != land)
                continue;
        }
        if (check != 0 && !(g_colonies[colony].status & 0x40))
            continue;
        dist = xy_dist(x - g_colonies[colony].x, y - g_colonies[colony].y);
        if (dist <= bestd) {
            best = colony;
            bestd = dist;
        }
    }
    g_nearest_dist = bestd;
    set_colony(best);
    return best;
}
