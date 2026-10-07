/* PORT: this file's own declarations of move_unit removed: the header declares it from the definition (W1 step 3). */
/* move_a_unit: candidate for 1020:dfb7 (106 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sends a unit one square in direction d, through the eight-direction tables at SEG27:0x0 and
 * SEG27:0x9. A direction of -1 becomes 8, the table's ninth entry, which is no step at all.
 *
 * RENAMED 2026-09-26 from unit_move_dir: the Mac build's move_a_unit(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller indian_unit; caller plan_move; callee move_unit), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` */

void move_a_unit(int u, int dir)
{
    if (dir == -1)
        dir = 8;
    move_unit(u, g_units[u].x + g_dir_dx[dir], g_units[u].y + g_dir_dy[dir]);
}
