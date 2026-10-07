/* buy_mercs: candidate for 1038:774c (668 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Before the revolution, one turn in 21 a random European power (SEG20:0x77d8) that is the
 * player or at peace with them (pair flag 0x40) offers mercenaries: 1-3 regulars plus either
 * one more regular or 1-2 artillery (counts in data segment 26, +0x36/+0x38/+0x3c), priced
 * (0-6)*100 + difficulty*200 + 800 apiece with artillery counting double. The offer is
 * spelled out ("2 Regulars, Artillery") and MERCENARIES asked when the treasury can pay;
 * answer 2 pays and lands them as an intervention force.
 *
 * RENAMED 2026-09-26 from offer_mercenaries: the Mac build's buy_mercs(void), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DoHuman; callee get_treaty; callee popm; callee say_number; callee say_string), the runner-up gets 4, and the arity agrees (0).
 */

#include "colonization.h"

#include "units.h"
#include "royal.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int get_treaty(int nation_a, int nation_b);

extern void space_string(char *buf);
extern void comma_string(char *buf);
extern void popup_country(int slot, int flag, int nation);

extern int popm(char *msg, int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void allied_invasion(int mercs);
void buy_mercs(void)
{
    int r;
    char buf[0x50];
    int i;
    int price;

    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE)
        return;
    if (rand_range(0, 20) != 0)
        return;
    g_game.merc_nation = rand_range(0, 3);
    if (g_game.merc_nation != g_game.player_nation && (get_treaty(g_game.merc_nation, g_game.player_nation) & 0x40) == 0)
        return;
    for (i = 0; i < 4; i++)
        g_merc[i] = 0;
    g_merc[ROYAL_REGULARS] = rand_range(1, 3);
    if (rand_range(0, 1) != 0)
        g_merc[ROYAL_REGULARS]++;
    else {
        g_merc[ROYAL_ARTILLERY] = 1;
        if (rand_range(0, 1) == 0)
            g_merc[ROYAL_ARTILLERY]++;
    }
    price = rand_range(0, 6) * 100 + g_game.difficulty * 200 + 800;
    price = ((g_merc[ROYAL_CAVALRY] + g_merc[ROYAL_ARTILLERY]) * 2 + g_merc[ROYAL_REGULARS]) * price;
    buf[0] = 0;
    say_number(buf, g_merc[ROYAL_REGULARS]);
    space_string(buf);
    /* The mercenaries offered are DRAGOONS although the slot they fill is ROYAL_REGULARS --
       which is why this file names the dragoons word where revolution_check names the regulars. */
    say_string(buf, g_unit_types[UNIT_DRAGOONS].name);
    if (g_merc[ROYAL_CAVALRY] != 0) {
        comma_string(buf);
        say_string(buf, g_unit_types[UNIT_CAVALRY].name);
    }
    if (g_merc[ROYAL_ARTILLERY] != 0) {
        comma_string(buf);
        if (g_merc[ROYAL_ARTILLERY] > 1) {
            say_number(buf, g_merc[ROYAL_ARTILLERY]);
            space_string(buf);
        }
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
