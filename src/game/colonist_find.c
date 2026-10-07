/* colonist_find: candidate for 1018:627f (92 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colonist_find
 *
 * Searches the 5x5 colony grid for the cell whose get_field value is v,
 * returning its column and row through the pointers; 1 when found.  The
 * helper is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from find_colony_grid_cell: the Mac build's colonist_find(short, short*, short*), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller colony_select_profession; caller pick_up_colonist; callee get_field), the runner-up gets 1, and the arity agrees (3).
 */

#include "colonization.h"

extern int g_stub_a;

extern char get_field(int x, int y);

int colonist_find(char colonist, int *px, int *py)
{
    int found = 0;
    register int j;
    register int i;

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++) {
            if (get_field(i, j) == colonist) {
                *px = i;
                *py = j;
                found = 1;
            }
        }
    }
    return found;
}
