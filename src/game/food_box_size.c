/* food_box_size: candidate for 1018:6222 (93 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: food_box_size
 *
 * Cell size 20: returns 200 (ten cells), stores 20 through the second
 * pointer and min(20*13 - 1, 118) through the first.  min_int is
 * -called and defined in-file.
 *
 * RENAMED 2026-09-26 from colony_grid_metrics: the Mac build's food_box_size(short*, short*), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in new_turn_colony -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

int food_box_size(int *pa, int *pb)
{
    register int n = 20;
    int r = n * 10;
    int t;

    if (pb != 0)
        *pb = n;
    if (pa != 0) {
        t = n * 12 + n - 1;
        *pa = min_int_1018_b329(t, 0x76);
    }
    return r;
}
