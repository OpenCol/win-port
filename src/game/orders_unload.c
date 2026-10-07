/* orders_unload: candidate for 1008:0e05 (136 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: orders_unload
 *
 * Opens the colony under the selected unit: nothing happens when there is no colony there,
 * and the window is only relaid out when the unit check returns zero.
 *
 * The twin of `orders_load` two bytes above it, and the only difference
 * is which check it makes: `perform_orders_unload(unit, 0)` here against `perform_orders_load(unit)` there.
 * Two entry points onto the same colony screen with different guards, written from one
 * template -- which the identical `goto out` shape and identical frame both confirm.
 *
 * RENAMED 2026-09-26 from open_colony_under_unit_1008_0e05: the Mac build's orders_unload(void), bound by call-graph agreement: 4 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee Redraw; callee perform_orders_unload; callee set_colony), the runner-up gets 3; and by call-sequence alignment: in GameBrains_KeyboardEvent the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says. The two signals agree on which of the pair is load and which unload.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;   /* SEG20:0x1566 */

extern void set_colony(int colony);
extern int perform_orders_unload(int u, int flag);
#include "classes.h"
extern void GameWin_Redraw(GameWin *w);

void orders_unload(void)
{
    int x;
    int y;
    int sel;
    int colony;

    sel = g_game.selected_unit;
    x = g_units[sel].x;
    y = g_units[sel].y;
    colony = find_colony(x, y);
    if (colony < 0)
        goto out;
    set_colony(colony);
    if (perform_orders_unload(sel, 0) != 0)
        goto out;
    GameWin_Redraw((GameWin *)&g_stat_win);
out:
    ;
}
