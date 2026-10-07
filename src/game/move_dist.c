/* move_dist: candidate for 1020:c005 (237 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether a path runs from (tx, ty) to (x, y), for find_4. The same square
 * is 0 at once, and a target 8 or more squares away on either axis is -1 without a search.
 * Otherwise the mode word at DGROUP:0x10a0 is set from `want` (0xd or 1), the destination left
 * in 0x4bf0/0x4bf2, and the search at 1020:b728 run with the moving nation at 0x10a4 cleared to
 * -1 for its duration; a success answers the value it left in 0x4bf4.
 *
 * Every distance is spelled out as `d > 0 ? d : ~d + 1` over a subtraction (rules 11 and 19),
 * once for the test and once for each arm. The trailing nine bytes are the dead second epilogue.
 *
 * RENAMED 2026-09-26 from grid_path_check: the Mac build's move_dist(short, short, short, short, short, short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (caller collect_stats; caller find_4; callee close_move), the runner-up gets 1; and by call-sequence alignment: in find_4 the calls either side align and this is the one unbound callee left in that gap (2.0 votes, next 0.0); the arity agrees (6), and the body does what the Mac name says.
 */

#include "colonization.h"

#define ABSDIFF(a, b) ((a) - (b) > 0 ? (a) - (b) : ~((a) - (b)) + 1)

/* same object: 4-byte `push cs; call ` */
extern int close_move(int x, int y, int limit);

int move_dist(int tx, int ty, int x, int y, int want, int limit)
{
    int r;
    int saved;

    r = -1;
    if (ABSDIFF(tx, x) == 0 && ABSDIFF(ty, y) == 0) {
        r = 0;
    } else {
        if (ABSDIFF(tx, x) >= 8 || ABSDIFF(ty, y) >= 8)
            goto out;
        g_path_unit_type = want != 0 ? 0xd : 1;
        g_grid_x = x;
        g_grid_y = y;
        saved = g_moving_nation;
        g_moving_nation = -1;
        r = close_move(tx, ty, limit);
        g_moving_nation = saved;
        if (r >= 0)
            r = g_path_result;
    }
out:
    return r;
}
