/* siege_check: candidate for 1018:7fe7 (388 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: siege_check
 *
 * Looks at the eight tiles around the current colony (SEG27 direction
 * tables) for units of the four European nations, taking each unit's
 * stack_check(u, 10) value: the colony's own nation's units add to *own
 * (as does a unit on the colony tile itself), other nations' add to *total
 * unless the nation pair carries flag 0x40, and the nation with the largest
 * value goes to *best.  Returns max(0, total - own).  Any out-pointer may be
 * null.  max_int is -called, so it is defined in-file.
 *
 * RENAMED 2026-09-26 from colony_trade_summary: the Mac build's siege_check(short*, short*, short*), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller job_change_legal; callee get_treaty; callee stack_check; callee top_unit_in_square), the runner-up gets 2, and the arity agrees (3). The old name was wrong: the body totals foreign European units on the eight squares around the colony against the colony's own -- a siege test, not a trade one.
 */

#include "colonization.h"

#include "units.h"
#include "nations.h"
#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int stack_check(int u, int kind);
extern int get_treaty(int nation_a, int nation_b);

int siege_check(int *best_out, int *own_out, int *total_out)
{
    register int u;
    register int dir;
    int own;
    int total;
    int best;
    int v;
    int bestv;
    int x;
    int y;
    int nation;
    int diff;

    u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (u >= 0)
        own = stack_check(u, 10);
    else
        own = 0;
    best = -1;
    total = 0;
    bestv = 0;
    for (dir = 0; dir < 8; dir++) {
        x = g_cur_colony_rec->x + g_dir_dx[dir];
        y = g_cur_colony_rec->y + g_dir_dy[dir];
        u = top_unit_in_square(x, y);
        if (u >= 0) {
            nation = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
            if (nation >= NATION_EUROPEAN_COUNT)
                continue;
            v = stack_check(u, 10);
            if (g_cur_colony_rec->nation == nation) {
                own += v;
            } else {
                if (get_treaty(g_cur_colony_rec->nation, nation) & 0x40)
                    continue;
                total += v;
                if (v >= bestv) {
                    bestv = v;
                    best = nation;
                }
            }
        }
    }
    diff = total - own;
    diff = max_int_1018_b34f(0, diff);
    if (best_out != 0)
        *best_out = best;
    if (own_out != 0)
        *own_out = own;
    if (total_out != 0)
        *total_out = total;
    return diff;
}
