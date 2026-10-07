/* royal_edicts: candidate for 1038:7027 (535 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Before the revolution the King adds (difficulty * 8 + 10) to his war chest (the long at +0x22
 * of the 316-byte nation record), doubled after 1600, 1700 and 1750 (SEG20:0x778c); once it
 * reaches 1800 a unit kind is picked (regulars, cavalry, artillery or men-of-war -- whichever lags the
 * counts at SEG20:0x77dc..), type_of_army names it for the mother country
 * (SEG20:0x77d4), its count grows, and either KINGBUY is shown and the price paid with
 * the nation's modifier byte (DGROUP:0x4b4c) added at +0xe, or (during the revolution, which
 * this branch never reaches) KINGMOBILIZE with the player's name.
 *
 * +0x22 IS THE KING'S MONEY, NOT YOURS. Your gold is +0x2a, which `add_gold`
 * clamps to 0..999,999 and which twenty files name. This field was called `treasury` here and
 * `tax_take` in `new_turn_colony` -- and both were half right: the King's chest grows by the
 * tax you pay (`new_turn_colony` does `+= tax` on this very field) and by the per-turn
 * allowance below, and he SPENDS it, 1800 at a time, on the expeditionary force. A running total
 * is not something you subtract from, which is what settles it.
 *
 * RENAMED 2026-09-26 from king_mobilises_forces: the Mac build's royal_edicts(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller rebel_check; caller revolution_check; callee popm; callee popup_say; callee set_europe), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "royal.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int type_of_army(int kind, int nation);
extern void popup_say(int slot, int word);
extern int popm(char *msg, int n);
extern void set_mood(int kind);

void royal_edicts(int nation)
{
    register int kind;
    int add;
    int total;
    int code;

    set_europe(nation);
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
        add = (g_game.difficulty << 3) + 10;
        if (g_game.year >= 1600)
            add += add;
        if (g_game.year >= 1700)
            add += add;
        if (g_game.year >= 1750)
            add += add;
        g_cur_nation_rec->royal_gold += add;
        if (g_cur_nation_rec->royal_gold < 0x708L)
            goto done;
    } else
        goto done;
    kind = 0;
    if ((g_game.royal_forces[ROYAL_REGULARS] + 2) / 3 > g_game.royal_forces[ROYAL_CAVALRY])
        kind = 1;
    if (g_game.royal_forces[ROYAL_REGULARS] / 4 > g_game.royal_forces[ROYAL_ARTILLERY])
        kind = 3;
    total = g_game.royal_forces[ROYAL_REGULARS] + g_game.royal_forces[ROYAL_CAVALRY] + g_game.royal_forces[ROYAL_ARTILLERY];
    if ((total + 5) / 10 > g_game.royal_forces[ROYAL_MAN_O_WAR])
        kind = 2;
    code = type_of_army(kind, g_game.mother_country_idx);
    g_game.royal_forces[kind]++;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
        popup_say(0, g_unit_types[code].name);
        popm("KINGBUY", 1);
        g_cur_nation_rec->royal_gold -= 0x708L;
        g_cur_nation_rec->bells_total += g_nation_free_colonists[nation];
    } else {
        set_mood(3);
        set_name_slot_1038_8775(0, g_nation_text[g_game.player_nation].country);
        popup_say(1, g_unit_types[code].name);
        popup_say(2, g_nation_name_word[g_game.player_nation]);
        popm("KINGMOBILIZE", 1);
    }
done:
    ;
}
