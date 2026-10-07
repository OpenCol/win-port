/* PORT: this file's own declarations of special_kill removed: the header declares it from the definition (W1 step 3). */
/* special_kill_stack: candidate for 1020:345a (90 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Runs special_kill on every unit in u's stack, starting from the chain's tail. When
 * it reports that a unit went, unit indices above that one have shifted down by one, so both
 * the saved next index and the caller's unit index in `winner` are decremented to keep pointing at the
 * same units.
 *
 * RENAMED 2026-09-26 from unit_stack_each: the Mac build's special_kill_stack(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller combat_fight; callee next_unit; callee special_kill; callee top_unit), the runner-up gets 2, and the arity agrees (5).
 */

#include "colonization.h"

extern int top_unit(int u);
extern int next_unit(int u);

/* same object: 4-byte `push cs; call ` */

void special_kill_stack(int u, int winner, int show, int x, int y)
{
    int n;

    n = top_unit(u);
    for (u = n; u >= 0; u = n) {
        n = next_unit(u);
        if (special_kill(u, winner, show, x, y)) {
            if (n > u)
                n--;
            if (winner > u)
                winner--;
        }
    }
}
