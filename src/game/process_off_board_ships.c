/* PORT: this file's own declarations of transit_class removed: the header declares it from the definition (W1 step 3). */
/* process_off_board_ships: candidate for 1030:3397 (770 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: process_off_board_ships
 *
 * The treasure fleet reaches Europe and is cashed in. Two passes over two different
 * off-board squares, and the money moves in the second.
 *
 * The first pass walks the chain at `(g_game.turn_nation - 16, g_game.turn_nation - 16)` looking
 * for a SHIP (type 0xd..0x12) belonging to the player, remembers the first one that is not
 * in state 2, and raises a flag if it is carrying anything. The second pass walks the
 * Europe chain at `(g_game.turn_nation - 20, g_game.turn_nation - 20)` -- the square
 * `num_cargo_vessels` and `europe_new_wars` both use -- and for every unit of type 0xa
 * (the treasure train) settles up:
 *
 *   amount   = spec * 100                       the treasure the train carries
 *   pct      = min(0x32, the nation's byte +1)  the King's cut, capped at 50%
 *   cut      = amount * pct / 100
 *   the nation's +0x22 += cut, +0x26 += amount - cut, and GOLD (+0x2a) += amount - cut
 *
 * The three text arguments are set in slots 0, 1 and 2 -- gross, percentage, net -- and
 * LOOTCASH (DGROUP:0x145f, read out of the shipped image) is the message. Then the train's
 * record is freed, and BOTH saved indices are moved down if they were above it, because
 * `delete_unit` compacts the array. That is the same fix-up
 * `kill_stack` needed, and doing it twice on one free is what made the
 * idiom recognisable.
 *
 * If a ship was found and the nation is a human-played European one, hint 9 fires when the flag
 * is up, and DGROUP:0xdc6/0xdc8 are set to 1 and the ship's index.
 *
 * TWO SPELLINGS THE BYTES INSISTED ON.
 *
 * `amount` is `amt = spec; amt *= 100;` and NOT `amt = spec * 100;`. The compound
 * assignment on a register variable emits `MOV AX,100 / IMUL DI` -- the one-operand
 * 16x16 form -- where the explicit form emits the three-operand `IMUL AX,AX,100`. Four
 * other spellings were probed (`100 * x`, casts to long both ways, the product passed
 * straight to the callee) and all four give the three-operand form. This is rule 39.
 *
 * And `SEG20:0x7798` is read as a WORD here, `CMP AX,[0x7798]` against the nation index.
 * `matched/game/MapWin_LeftMouseUp.c` declares the same address `char g_game.view_nation`
 * and matches that way. Another per-module declaration difference, like `g_cur_colony_rec`
 * in `create_colony`.
 *
 * Five callees arrive as the compiler's own 4-byte PUSH CS / CALL NEAR and are stubs here.
 *
 * RENAMED 2026-09-26 from treasure_fleet_arrives: the Mac build's process_off_board_ships(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller new_turn_bookkeeping; callee force_music; callee popm; callee popup_say), the runner-up gets 3, the arity agrees (0), and the Mac body is 1.01x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says.
 */

#include "colonization.h"

#include "control.h"
#include "woodcut.h"
#include "nations.h"
#include "units.h"
#include "records.h"

extern Nation g_nations[];   /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern void tell_unit_in_europe(int u);

extern void popup_say(int slot, int v);
extern void force_music(int what);
extern int popm(char *msg, int n);
extern void delete_unit(int u);


extern void arrivals_to_mapboard(void);

void process_off_board_ships(void)
{
    int found;
    int flag;
    int next;
    int pct;
    long cut;
    register int u;
    register unsigned amt;

    found = -1;
    flag = 0;
    transit_class(-28, -32);
    transit_class(-24, -28);
    u = top_unit_in_square(g_game.turn_nation - 16, g_game.turn_nation - 16);
    while (u >= 0) {
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && found < 0
            && g_game.turn_nation == g_game.view_nation) {
            tell_unit_in_europe(u);
            if (g_units[u].orders != ORDERS_TRADE_ROUTE) {
                found = u;
                if (g_units[u].cargo_count != 0)
                    flag = 1;
            }
        }
        u = next_unit(u);
    }
    transit_class(-16, -20);
    transit_class(-12, -16);
    u = top_unit_in_square(g_game.turn_nation - 20, g_game.turn_nation - 20);
    while (u >= 0) {
        next = next_unit(u);
        if (g_units[u].type == UNIT_TREASURE_TRAIN) {
            amt = (unsigned char)g_units[u].spec;
            amt *= 100;
            set_text_arg_u_1030_eafc(0, amt);
            pct = g_nations[g_game.turn_nation].tax;
            pct = min_int_1030_eb20(0x32, pct);
            cut = (long)amt * pct / 100;
            set_text_arg_i_1030_eb46(1, pct);
            amt -= (unsigned)cut;
            set_text_arg_u_1030_eafc(2, amt);
            popup_say(0, get_adjective((unsigned char)(g_units[u].flags & UNIT_NATION_MASK)));
            popup_say(1, g_nation_name_word[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]);
            g_nations[g_game.turn_nation].gold += amt;
            g_nations[g_game.turn_nation].net_take += amt;
            g_nations[g_game.turn_nation].royal_gold += cut;
            force_music(0x24);
            popm("LOOTCASH", 2);
            delete_unit(u);
            if (next > u)
                next--;
            if (found > u)
                found--;
        }
        u = next;
    }
    if (found >= 0 && g_game.turn_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_game.turn_nation].control == CONTROL_HUMAN) {
        if (flag != 0)
            woodcut(WOODCUT_CARGO_FROM_THE_NEW_WORLD);
        g_show_europe = 1;
        g_europe_arrival_unit = found;
    }
    arrivals_to_mapboard();
}
