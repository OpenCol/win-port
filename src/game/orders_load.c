/* orders_load: candidate for 1008:0d7f (134 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: orders_load
 *
 * Selects the colony under the selected unit (`set_colony`) and runs `perform_orders_load` on the unit: nothing happens when there is no colony there, and the status window at SEG20:0x1566 is redrawn only when that returns zero.
 *
 * RENAMED 2026-09-26 from open_colony_under_unit_1008_0d7f: the Mac build's orders_load(void), bound by call-graph agreement: 4 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee Redraw; callee perform_orders_load; callee set_colony), the runner-up gets 3; and by call-sequence alignment: in GameBrains_KeyboardEvent the calls either side align and this is the one unbound callee left in that gap (0.5 votes, next 0.0); the arity agrees (0), and the body does what the Mac name says. The two signals agree on which of the pair is load and which unload.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;   /* SEG20:0x1566 */

extern void set_colony(int colony);
extern int perform_orders_load(int u);
#include "classes.h"
extern void GameWin_Redraw(GameWin *w);

void orders_load(void)
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
    if (perform_orders_load(sel) != 0)
        goto out;
    GameWin_Redraw((GameWin *)&g_stat_win);
out:
    ;
}
