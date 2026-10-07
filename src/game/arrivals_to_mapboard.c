/* arrivals_to_mapboard: candidate for 1030:330b (140 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks the unit chain standing on one square and processes every SHIP on it -- types 0xd to
 * 0x12, six consecutive types -- restarting the walk from the head after each one, because
 * the two things it does to a ship can take it off the chain.
 *
 * The restart is the interesting part and it is a flag, not a `goto`: `again` starts at 1,
 * the head is re-fetched only while it is set, and the two actions set it again while
 * following the chain clears it. That is why `top_unit_in_square` is called inside the loop rather than
 * before it.
 *
 * The square is `g_game.turn_nation - 0x20` for BOTH coordinates -- the same word read twice and the same
 * constant subtracted twice, which the shipped code recomputes each time. A diagonal is an
 * odd thing to ask for and it is what the bytes say.
 *
 * The type test is on an unsigned byte: `CMP BYTE ...,0xd / JC` and `CMP BYTE ...,0x12 / JA`.
 *
 * RENAMED 2026-09-26 from ships_at_capital: the Mac build's arrivals_to_mapboard(void), bound by call-graph agreement: 2 of its Mac call edges already bound land on this function (caller process_off_board_ships; callee stack_to_board), the runner-up gets 1; the arity agrees (0), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern void stack_ship(int u);

/* same object: 4-byte `push cs; call ` */
extern void stack_to_board(int u);

void arrivals_to_mapboard(void)
{
    register int u;
    register int again;

    again = 1;
    do {
        if (again)
            u = top_unit_in_square(g_game.turn_nation - 0x20, g_game.turn_nation - 0x20);
        if (u >= 0) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                stack_ship(u);
                stack_to_board(u);
                again = 1;
            } else {
                u = next_unit(u);
                again = 0;
            }
        }
    } while (u >= 0);
}
