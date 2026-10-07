/* field_index: candidate for 1018:411c (81 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Which of the twenty ring offsets (SEG27:0x12/0x27) equals (x - 2, y - 2);
 * -1 when none.
 *
 * RENAMED 2026-09-26 from ring_index_of: the Mac build's field_index(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller colony_field_status; caller get_field; caller set_field), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

int field_index(int x, int y)
{
    int r = -1;
    int i;

    x -= 2;
    y -= 2;
    for (i = 0; i < 20; i++) {
        if (g_ring_dx[i] == (signed char)x && g_ring_dy[i] == (signed char)y) {
            r = i;
            break;
        }
    }
    return r;
}
