/* resource_bonus: candidate for 1018:64ba (205 bytes, 3 callers).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * A hand-written table of (a, b) pairs: fourteen tests, each adding 1..3 or forcing -1.
 *
 * RENAMED 2026-09-26 from pair_score_table: the Mac build's resource_bonus(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in compute_yield -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

int resource_bonus(int resource, int goods)
{
    int x = resource;
    int y = goods;
    int r = 0;

    if (x == 9 && y == 0) r += 2;
    if (x == 1 && y == 0) r += 2;
    if (x == 2 && y == 0) r += 2;
    if (x == 9 && y == 4) r += 2;
    if (x == 8 && y == 4) r += 3;
    if (x == 3 && y == 3) r = -1;
    if (x == 4 && y == 2) r = -1;
    if (x == 5 && y == 1) r = -1;
    if (x == 10 && y == 5) r += 2;
    if (x == 6 && y == 6) r += 3;
    if (x == 13 && y == 6) r += 2;
    if (x == 6 && y == 7) r += 1;
    if (x == 12 && y == 7) r += 2;
    if (x == 7 && y == 8) r += 3;
    return r;
}
