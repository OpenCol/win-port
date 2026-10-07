/* indian_unit: candidate for 1030:9c93 (61 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Asks indian_move for a direction for unit u. Any direction but 8 moves the unit that
 * way through move_a_unit; 8 instead refreshes the unit's rating. The `>= 0` test
 * on the 8 arm can never fail, and the shipped code makes it anyway.
 *
 * RENAMED 2026-09-26 from unit_raid_or_rate: the Mac build's indian_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller indian_turn; callee end_unit_turn; callee indian_move), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

extern void move_a_unit(int u, int dir);
extern void end_unit_turn(int u);

/* same object: 4-byte `push cs; call ` */
extern int indian_move(int u);

void indian_unit(int u)
{
    int dir;

    dir = indian_move(u);
    if (dir != 8)
        move_a_unit(u, dir);
    else if (dir >= 0)
        end_unit_turn(u);
}
