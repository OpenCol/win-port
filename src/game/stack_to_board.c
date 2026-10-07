/* stack_to_board: candidate for 1030:310d (510 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * PUTS A UNIT ON ITS DESTINATION SQUARE (+9, +0xa) or the nearest one that will take it. The
 * search is by rings: at radius r, the two horizontal edges of the ring are walked (x from
 * x0 - r to x0 + r, at y0 - r and y0 + r), then the two vertical edges, and r grows until
 * `square_ok` accepts a square or r passes the larger map dimension. The corners
 * are visited twice, which costs two tests per ring and nothing else.
 *
 * IF NOTHING IS FOUND the unit still lands on its destination, and whatever stack is already
 * there is freed -- `top_unit_in_square` on the unchanged (fx, fy) and `kill_stack`. Then the unit
 * is unlinked, relinked at the square, joined to its nation's list, and the square explored;
 * and if the player is watching that nation, the view scrolls to it or repaints a 7x7 block
 * around it, and the viceroy message runs.
 *
 * THE RING OFFSET IS A NESTED TERNARY AND BOTH HALVES ARE WRITTEN "BACKWARDS":
 * `i != 0 ? (i >= 0 ? r : ~r + 1) : 0`. The zero arm last and the negation second are what the
 * jumps say -- the natural `i == 0 ? 0 : (i < 0 ? -r : r)` inverts both tests -- and `~r + 1`
 * is the same two's-complement negation `map_dist` uses.
 *
 * `x` and `y` are two `register` locals that swap roles between the two edge loops: the scan
 * variable in one is the computed one in the other.
 *
 * RENAMED 2026-09-26 from unit_arrive_nearest: the Mac build's stack_to_board(short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (callee explore_square; callee possibly_center; callee square_ok), the runner-up gets 1; the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */
extern int top_unit_in_square(int x, int y);
extern void kill_stack(int u);
extern void unspot_stack(int u);
extern void relocate_stacked_units(int u, int x, int y);
extern void possibly_spot_stack(int u);

extern int possibly_center(int x1, int y1, int x2, int y2, int m);
extern void DrawMapRegion(int x, int y, int w, int h, int flag);
extern void tell_unit_from_europe(int u);

/* same object: 4-byte `push cs; call ` */
extern int square_ok(int u, int x, int y);

void stack_to_board(int u)
{
    int x0;
    int y0;
    int j;
    int i;
    int fx;
    int fy;
    int found;
    int r;
    int other;
    register int x;
    register int y;

    found = 0;
    r = 0;
    fx = x0 = g_units[u].dest_x;
    fy = y0 = g_units[u].dest_y;
    do {
        for (x = x0 - r; found == 0 && x0 + r >= x; x++) {
            for (i = -1; i <= 1; i += 2) {
                y = (i != 0 ? (i >= 0 ? r : ~r + 1) : 0) + y0;
                if (square_ok(u, x, y) != 0) {
                    found = 1;
                    fx = x;
                    fy = y;
                }
            }
        }
        for (y = y0 - r; found == 0 && y0 + r >= y; y++) {
            for (j = -1; j <= 1; j += 2) {
                x = (j != 0 ? (j >= 0 ? r : ~r + 1) : 0) + x0;
                if (square_ok(u, x, y) != 0) {
                    found = 1;
                    fx = x;
                    fy = y;
                }
            }
        }
        r++;
    } while (found == 0 && max_int_1030_ead6(g_map_size.w, g_map_size.h) > r);
    if (found == 0) {
        other = top_unit_in_square(fx, fy);
        kill_stack(other);
    }
    unspot_stack(u);
    relocate_stacked_units(u, fx, fy);
    possibly_spot_stack(u);
    explore_square(u);
    if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == ((unsigned char)g_game.view_nation)) {
        if (possibly_center(fx, fy, fx, fy, 0) == 0)
            DrawMapRegion(fx - 3, fy - 3, 7, 7, 1);
        tell_unit_from_europe(u);
    }
}
