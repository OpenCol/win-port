/* perform_orders_move_to: candidate for 1028:ae3d (822 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ONE STEP OF A STANDING MOVE -- `foreign_unit`'s order 10, after order 8's
 * work and order 9's colony. `move_to` picks the direction, 0..7 into the `signed char`
 * tables at SEG27:0x0 and SEG27:0x9; anything else means there is nowhere to go and the
 * orders are cleared, EXCEPT direction 8 (stay put) on a unit whose orders are 2.
 *
 * TWO WAYS TO TAKE THE STEP. A human nation (index below 4 whose controller byte
 * SEG20:0x7841 -- +1 of the 0x34-byte record at 0x7840 -- is clear) goes through `execute_move_unit`, which takes only the DELTA
 * and works on the selected unit -- a non-zero return cancels the orders. Everyone else goes
 * through `move_unit(u, x + dx, y + dy)`, an absolute move of a named unit.
 * That callee's own prologue reads `[bp+6]` as the unit index, so the split is three `int`s
 * and not the `long`+`int` the call widths allow (§119).
 *
 * IF THE UNIT DIED THE FUNCTION IS OVER: `g_game.unit_count` is sampled before the move and
 * compared after, and a change is a `goto` to the last statement -- the shipped uses the
 * two-jump form there and a DIRECT `Jcc` for every `if` that ends away, which is how
 * the two are told apart (rule 12).
 *
 * ARRIVAL is `x == +9 && y == +0xa`, the unit's destination. On terrain 0x1a, orders not 12,
 * and either a human player or mission 0x45, the stack-arrival handler runs -- but once bit 0
 * of SEG20:0x7784 is set only the mother country's type-0x12 unit still qualifies. The
 * viceroy message follows only when the arriving unit belongs to the nation being watched.
 *
 * Then three tidy-ups that run on ANY arrival: a type-2 unit has +0x11 cleared and +0x12 set
 * to 0xff, orders 11 refreshes the rating, and anything but orders 2 or 12 ends with the
 * orders cleared.
 *
 * DGROUP:0x10a4 IS THE NATION WHOSE MOVE IS BEING DRAWN. It is set to the unit's nation on
 * entry -- or to -1 for orders 12, which is what makes that order's move invisible -- and
 * back to -1 on every exit. Segment 1020 is the only reader: it compares it against 4 and
 * indexes with it.
 *
 * `u` is a `register` parameter in SI; `d` is a PLAIN local the allocator promotes to DI and
 * so must be declared before `n0`, which is the one that spills (rule 53's fourth tier).
 * `(g_game.game_flags & 1) == 0` folds to one `TEST BYTE` where `!(...)` would load and promote
 * (rule 59), and `f7` is SIGNED -- the 0x45 comparison goes through `CBW`.
 *
 * RENAMED 2026-09-26 from mission_goto_step: the Mac build's perform_orders_move_to(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller foreign_unit; caller perform_orders_route; callee end_unit_turn; callee move_unit; callee terrain_at), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "control.h"
#include "gameflag.h"
#include "terrain.h"
#include "nations.h"
#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern int move_to(int u);
extern int execute_move_unit(int dx, int dy);
extern void move_unit(int u, int x, int y);

extern void send_unit_to_europe(void);
extern void tell_unit_to_europe(int u);
extern void end_unit_turn(int u);

void perform_orders_move_to(register int u)
{
    int dir;
    int unit_count;

    if (g_units[u].orders == 0xc)
        g_moving_nation = -1;
    else
        g_moving_nation = NATION(g_units[u]);
    dir = move_to(u);
    if (dir >= 0 && dir < 8) {
        unit_count = g_game.unit_count;
        if (NATION(g_units[u]) < 4 && g_nation_text[NATION(g_units[u])].control == CONTROL_HUMAN) {
            if (execute_move_unit(g_dir_dx[dir], g_dir_dy[dir]) != 0)
                g_units[u].orders = ORDERS_NONE;
        } else {
            move_unit(u, g_units[u].x + g_dir_dx[dir],
                                       g_units[u].y + g_dir_dy[dir]);
        }
        if (g_game.unit_count != unit_count)
            goto done;
        if (g_units[u].x == g_units[u].dest_x && g_units[u].y == g_units[u].dest_y) {
            if (terrain_at(g_units[u].x, g_units[u].y) == TERRAIN_SEA_LANE
                && g_units[u].orders != 0xc
                && ((g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN)
                    || (int)g_units[u].mission == 0x45)
                && ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0
                    || (g_game.turn_nation == g_game.mother_country_idx
                        && g_units[u].type == UNIT_MAN_O_WAR))) {
                g_game.selected_unit = u;
                send_unit_to_europe();
                if (NATION(g_units[u]) == ((unsigned char)g_game.view_nation))
                    tell_unit_to_europe(g_game.selected_unit);
            }
            if (g_units[u].type == UNIT_PIONEERS) {
                g_units[u].hold.w.work_turns = 0;
                g_units[u].hold.w.work_dir = 0xff;
            }
            if (g_units[u].orders == ORDERS_MISSION)
                end_unit_turn(u);
            if (g_units[u].orders != ORDERS_TRADE_ROUTE && g_units[u].orders != 0xc)
                g_units[u].orders = ORDERS_NONE;
        }
    } else {
        if (dir != 8 || g_units[u].orders != ORDERS_TRADE_ROUTE)
            g_units[u].orders = ORDERS_NONE;
    }
done:
    g_moving_nation = -1;
}
