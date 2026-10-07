/* intervention: candidate for 1038:6488 (321 bytes).  Verdict: RELOCATABLE.
 * symbol: intervention
 *
 * The intervention event: a foreign power declares for the rebels. It picks which colony
 * the announcement names, fills four text slots, shows the message and latches bit 1 of the
 * flags word at SEG20:0x7784 so it can only happen once.
 *
 * The colony it picks is the largest eligible one: every colony of nation `g_game.player_nation`
 * whose flag bit 0x40 is set, keeping the one with the greatest `+0x1f`, starting from -1
 * so that a nation with no eligible colony leaves the index at 0. Two text resources carry
 * the message, both read out of the shipped image -- `FRIEND` at DGROUP:0x19ef and
 * `INTERVENTION` at DGROUP:0x19f6 -- and the chosen colony's NAME goes into slot 3 from
 * `&g_colonies[best].name`.
 *
 * NAME: the Macintosh build's `intervention(void)`, adopted on arity and role. This
 * function takes no arguments and that symbol takes none. The obvious alternative is
 * already spoken for and does not fit: `allied_invasion` at `1038:5ec0` is 1,480
 * bytes, takes ONE argument and uses the `INTERVENE` resource, so it is the force actually
 * landing rather than the declaration. Two functions, two resources, two arities.
 *
 * FOUR SPELLINGS THE BYTES FIXED, all of them rules already written down.
 *
 * The flag test is `!(x & 0x40)` and not `(x & 0x40) == 0`: the shipped code loads the byte,
 * promotes it with `MOV AH,0` and then tests `AL`, where the `== 0` form tests memory
 * directly and never loads.
 *
 * The first two tests in the loop are `continue` and the third is a positive `if` with a
 * body -- `JLE` straight over the two assignments, where a third `continue` emits Borland's
 * invert-and-jump-over-a-jump (rule 12). The same split as `find_my_close_unit`.
 *
 * `g_game.player_nation` is an `int` that is COMPARED as a char: `PUSH WORD [0x779a]` when it is
 * passed on, `CMP AL,[0x779a]` when it is matched against a colony's nation byte. Declared
 * `char` the push becomes a load-and-sign-extend; declared `int` without the cast the
 * compare becomes 16-bit.
 *
 * And two globals whose addresses are taken are declared ``: the shipped code pushes an
 * explicit segment immediate for each, where a declaration pushes `DS`.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void set_colony(int colony);
extern void force_mood(int mood);
extern void popup_country(int slot, int flag, int nation);
extern void text_search(void *ctx, char *name, int n);

extern void popup_say(int slot, int v);
extern int popm(char *msg, int n);

void intervention(void)
{
    int best;
    register int colony;
    register int bestc;

    best = -1;
    bestc = 0;
    for (colony = 0; g_game.colony_count > colony; colony++) {
        set_colony(colony);
        if ((char)g_cur_colony_rec->nation != (char)g_game.player_nation)
            continue;
        if (!(g_colonies[colony].status & 0x40))
            continue;
        if (g_cur_colony_rec->pop > (signed char)best) {
            best = g_cur_colony_rec->pop;
            bestc = colony;
        }
    }
    force_mood(3);
    popup_country(0, 1, g_game.intervention_nation);
    popup_country(1, 0, g_game.player_nation);
    text_search(g_game_txt, "FRIEND", g_game.intervention_nation);
    set_name_slot_1038_8775(2, g_text_buf);
    set_name_slot_1038_8775(3, g_colonies[bestc].name);
    popup_say(4, get_adjective(g_game.intervention_nation));
    popm("INTERVENTION", 1);
    g_game.game_flags |= 2;
}
