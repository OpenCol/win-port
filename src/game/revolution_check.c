/* revolution_check: candidate for 1038:7451 (763 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ONE TURN OF THE WAR OF INDEPENDENCE, for one side. `rebel_check` calls it once the
 * revolution flag (bit 0 of SEG20:0x7784) is set, for the player's nation and for the mother country.
 *
 * On the MOTHER COUNTRY's turn (SEG20:0x77d4) the King may first mobilise more forces (bit 3 of
 * the player's nation record), then counts what is left of the royal army at SEG20:0x77dc:
 * every regular, plus one for any cavalry at +2 and one for any artillery at +6. With anything
 * left the expedition lands; with nothing, the Tories rise instead.
 *
 * On the REBELS' turn, in order and only one of these:
 *   - the first time, the Continental Army is mobilised and bit 3 of the market record's first
 *     byte latches it;
 *   - once a power has intervened (bit 1 of SEG20:0x7784, set by `intervention`) and
 *     SEG20:0x77e8 is non-zero, the intervention force arrives;
 *   - otherwise, one turn in three, MERCENARIES are offered.
 *
 * The offer is `buy_mercs` (1038:774c, the next function) retuned for wartime: 2 to
 * `2 + (4 - difficulty) / 2` regulars plus a single cavalry or a single artillery, at
 * (0-6)*100 + difficulty*200 + 600 apiece against the peacetime 800, cavalry and artillery
 * counting double. Answer 2 pays and lands them through the same `allied_invasion(1)`.
 *
 * `!(g_cur_nation_rec->flags & 8)` and not `== 0`: the shipped promotes the byte (`MOV AH,0`) before
 * the `TEST AL,8`, which only the `!` spelling does -- the same as `!take` in
 * `stack_ship`. The early `return` on the dice is the one inline epilogue
 * (rule 44); every other arm falls to the shared one.
 *
 * RENAMED 2026-09-26 from independence_war_turn: the Mac build's revolution_check(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (callee allied_invasion; callee king_invasion; callee popm; callee popup_country; callee say_number), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "royal.h"
#include "gameflag.h"
#include "records.h"

extern Nation g_nations[];           /* 316 bytes each at SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);

extern void space_string(char *buf);
extern void comma_string(char *buf);
extern void popup_country(int slot, int flag, int nation);
extern int popm(char *msg, int n);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void royal_edicts(int nation);
extern void king_invasion(int nation);
extern void tory_unrest(int nation);
extern void revolution_mobilize(int nation);
extern void allied_invasion(int mercs);

void revolution_check(int nation)
{
    int left;
    int r;
    char buf[0x50];
    register int i;
    register int price;

    set_europe(nation);
    if (g_game.mother_country_idx == nation) {
        if (g_nations[g_game.player_nation].flags & 8)
            royal_edicts(nation);
        left = g_game.royal_forces[ROYAL_REGULARS] + (g_game.royal_forces[ROYAL_ARTILLERY] > 0) + (g_game.royal_forces[ROYAL_CAVALRY] > 0);
        if (left != 0)
            king_invasion(g_game.player_nation);
        else
            tory_unrest(g_game.player_nation);
    } else if (!(g_cur_nation_rec->flags & 8)) {
        revolution_mobilize(nation);
        g_cur_nation_rec->flags |= 8;
    } else if ((g_game.game_flags & GAMEFLAG_INTERVENTION) && g_game.force[ROYAL_MAN_O_WAR] != 0) {
        allied_invasion(0);
    } else {
        if (rand_range(0, 2) != 0)
            return;
        for (i = 0; i < 4; i++)
            g_merc[i] = 0;
        g_merc[ROYAL_REGULARS] = rand_range(2, ((4 - g_game.difficulty) >> 1) + 2);
        if (rand_range(0, 1) != 0)
            g_merc[ROYAL_CAVALRY] = 1;
        else
            g_merc[ROYAL_ARTILLERY] = 1;
        price = rand_range(0, 6) * 100 + g_game.difficulty * 200 + 600;
        price = ((g_merc[ROYAL_CAVALRY] + g_merc[ROYAL_ARTILLERY]) * 2 + g_merc[ROYAL_REGULARS]) * price;
        buf[0] = 0;
        say_number(buf, g_merc[ROYAL_REGULARS]);
        space_string(buf);
        say_string(buf, g_unit_types[UNIT_REGULARS].name);
        if (g_merc[ROYAL_CAVALRY] != 0) {
            comma_string(buf);
            say_string(buf, g_unit_types[UNIT_CAVALRY].name);
        }
        if (g_merc[ROYAL_ARTILLERY] != 0) {
            comma_string(buf);
            say_string(buf, g_unit_types[UNIT_ARTILLERY].name);
        }
        if ((long)price <= g_cur_nation_rec->gold) {
            popup_country(0, 0, g_game.merc_nation);
            set_text_arg_i_1038_86cc(0, price);
            set_name_slot_1038_8775(1, buf);
            r = popm("MERCENARIES", 1);
            if (r == 2) {
                g_cur_nation_rec->gold -= price;
                allied_invasion(1);
            }
        }
    }
}
