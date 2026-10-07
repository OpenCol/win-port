/* which_move: candidate for 1020:b6ab (125 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Turns a delta into one of the eight compass directions by matching the signs of its components against a pair of nine-entry tables; 8 means no direction.
 *
 * RENAMED 2026-09-26 from direction_of: the Mac build's which_move(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in move_to -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

int which_move(int dx, int dy)
{
    int dir = 8;
    int i;
    dx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    dy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);
    for (i = 0; i < 8; i++)
        if (g_dir_dx[i] == dx && g_dir_dy[i] == dy)
            dir = i;
    return dir;
}
