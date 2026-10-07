/* bearing_adjacent: candidate for 1040:7ca4 (51 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * 1 when direction a is one step either side of direction b on the
 * eight-point compass.
 *
 * RENAMED 2026-09-26 from is_adjacent_dir: the Mac build's bearing_adjacent(short, short), bound by call-sequence alignment (1.0 aligned gaps in indian_move leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 2.31x this one.
 */

#include "colonization.h"

int bearing_adjacent(int dir, int last_dir)
{
    int r = 0;

    if (((last_dir + 1) & 7) == dir || ((last_dir - 1) & 7) == dir)
        r = 1;
    return r;
}
