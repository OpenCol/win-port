/* free_galleon2: candidate for 1020:420d (595 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: free_galleon2
 *
 * A treasure train reaching a port with no galleon of its own. The treasure is the unit's
 * byte at +0x17 times 100.
 *
 * After independence (bit 0 of SEG20:0x7784) there is no King to pay: the whole sum is
 * cashed with "CASHTREASURE" and that is all. Before it, the King offers to carry the
 * treasure for a cut, and the offer is put with "KINGGALLEON2" or "KINGGALLEON3" depending
 * on nation flag 0xa; only answer 1 accepts, and anything else leaves the treasure train
 * standing.
 *
 * THE CUT IS THE TAX RATE, DOUBLED. Nation+1 is the tax percentage; without flag 0xa it is
 * doubled and then floored at `difficulty * 5 + 50`, so on the hardest setting the King never
 * takes less than 70 per cent. WITH the flag neither the doubling nor the floor applies and
 * the cut is the bare tax rate. Both are capped at 90. The flag is the Founding
 * Father FATHER_HERNAN_CORTES (bit 10) -- it changes the wording as well as the number -- and
 * combat_fight, move_unit and tribe_extortion read it too.
 *
 * What arrives goes into Nation+0x2a (gold) and Nation+0x26; what the King keeps goes into
 * Nation+0x22. The unit record is freed either way.
 *
 * FOUR SPELLINGS HAD TO BE EXACT (all rule 51/60 shapes):
 *   `treasure = unit->treasure; treasure *= 100;` -- one statement `f * 100` gives the
 *     three-operand `IMUL AX,AX,0x64` where the shipped loads 100 into AX and does the
 *     one-operand `IMUL DI`.
 *   `pct = pct * 2` gives `ADD AX,AX`; `pct = pct + pct` gives `ADD AX,[BP-6]`.
 *   `treasure -= (unsigned)amount` subtracts the long's low word in place (`SUB DI,[BP-0xa]`);
 *     without the cast the whole thing widens and comes back through AX.
 *   `nation` is a register LOCAL copied from the parameter, not a register parameter: that is
 *     what puts it in SI ahead of `treasure` in DI, since register locals outrank register
 *     parameters (rule 53) and the two locals take SI then DI in declaration order.
 *
 * The rejected answer jumps to the single epilogue rather than emitting its own, so the
 * source reaches the end through a `goto` and not a `return` (rule 44).
 *
 * RENAMED 2026-09-26 from sell_treasure_to_agents: the Mac build's free_galleon2(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller move_unit; callee delete_unit; callee has_father; callee new_mood; callee popm), the runner-up gets 6, and the arity agrees (2).
 */

#include "colonization.h"

#include "fathers.h"
#include "gameflag.h"
#include "sounds.h"
#include "records.h"

extern Nation g_nations[];             /* SEG20:0xc2f4 */

extern GameState g_game;   /* SEG20:0x7782 */
extern void popup_say(int slot, int word);
extern void popup_country(int slot, int flag, int nation);

extern void sound_play(int n);
extern int popm(char *msg, int n);
extern void new_mood(int mood);

extern void delete_unit(int u);

/* 5-byte NOP form, so extern (rule 47) */
extern int has_father(int nation, int bit);

/* same object: 4-byte `push cs; call ` */

extern void set_text_arg_char_1020_a973(int i, char v);

void free_galleon2(int u, int n)
{
    int r;
    int pct;
    long amount;
    char buf[0x50];
    register int nation;
    register unsigned treasure;

    nation = n;
    treasure = (unsigned char)g_units[u].spec;
    treasure *= 100;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) != 0) {
        set_text_arg_u_1020_a9de(0, treasure);
        popm("CASHTREASURE", 2);
        new_mood(2);
    } else {
        popup_say(0, g_difficulty_words[g_game.difficulty]);
        set_name_slot_1020_a94a(1, g_nation_text[nation].leader);
        popup_country(2, 0, nation);
        set_text_arg_char_1020_a973(0, g_nations[nation].tax);
        STRCPY(buf, "KINGGALLEON");
        if (has_father(nation, FATHER_HERNAN_CORTES) != 0)
            STRCAT(buf, "3");
        else
            STRCAT(buf, "2");
        sound_play(SOUND_MEETKING);
        r = show_text_dialog_1020_aa02(buf);
        if (r != 1)
            goto done;
        pct = g_nations[nation].tax;
        if (has_father(nation, FATHER_HERNAN_CORTES) == 0) {
            pct = pct * 2;
            pct = max_int_1020_a887(g_game.difficulty * 5 + 0x32, pct);
        }
        pct = min_int_1020_a8ad(0x5a, pct);
        amount = (long)treasure * pct / 100;
        set_text_arg_u_1020_a9de(0, treasure);
        set_text_arg_i_1020_a926(1, pct);
        treasure -= (unsigned)amount;
        set_text_arg_u_1020_a9de(2, treasure);
        popup_say(0, get_adjective(nation));
        popup_say(1, g_nation_name_word[nation]);
        new_mood(2);
        popm("LOOTCASH", 2);
        g_nations[nation].royal_gold += amount;
    }
    g_nations[nation].gold += treasure;
    g_nations[nation].net_take += treasure;
    delete_unit(u);
done:
    ;
}
