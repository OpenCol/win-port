/* siege_check_2: candidate for 1018:816b (339 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Like siege_check but for one nation: over the eight tiles around
 * the current colony, units of `nation` add their stack_check(u, 10) value
 * to *total and their (u, 11) value >> 3 to the returned sum, *best gets
 * `nation` itself when any of its units stands there and -1 otherwise (the
 * strongest-stack test stores the nation id, never a value); *own is the value of the unit on the colony
 * tile itself.  Any out-pointer may be null.
 *
 * RENAMED 2026-09-26 from colony_defence_summary: the Mac build's siege_check_2(short*, short*, short*, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller negotiate_europe; callee stack_check; callee top_unit_in_square), the runner-up gets 1, and the arity agrees (4).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int stack_check(int u, int kind);

int siege_check_2(int *best_out, int *own_out, int *total_out, int nation)
{
    register int u;
    register int dir;
    int total;
    int best;
    int tally;
    int bestv;
    int x;
    int y;
    int nat;
    int own;
    int sum;

    u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (u >= 0)
        own = stack_check(u, 10);
    else
        own = 0;
    best = -1;
    total = 0;
    sum = 0;
    bestv = 0;
    for (dir = 0; dir < 8; dir++) {
        x = g_cur_colony_rec->x + g_dir_dx[dir];
        y = g_cur_colony_rec->y + g_dir_dy[dir];
        u = top_unit_in_square(x, y);
        if (u >= 0) {
            nat = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
            tally = stack_check(u, 10);
            if (nat != nation)
                continue;
            else {
                sum += stack_check(u, 11) >> 3;
                total += tally;
                if (tally >= bestv) {
                    bestv = tally;
                    best = nat;
                }
            }
        }
    }
    if (best_out != 0)
        *best_out = best;
    if (own_out != 0)
        *own_out = own;
    if (total_out != 0)
        *total_out = total;
    return sum;
}
