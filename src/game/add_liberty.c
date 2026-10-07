/* add_liberty: candidate for 1020:a6c3 (363 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Credits nation `nation` with `bells` liberty bells and spends them. Both counters at
 * +0xc and +0xe of the 316-byte nation record rise by the same amount; only +0xc is ever
 * spent, so +0xe is the running total the score and the rebel percentage are read from.
 *
 * What the bells buy depends on one bit. Before independence (bit 0 of SEG20:0x7784
 * clear) a nation with no Founding Father pending picks one, and when the pot reaches
 * `freedom_level(nation)` the pending father joins and the pot is emptied.
 * After independence is declared the same pot funds the foreign INTERVENTION instead, and
 * `freedom_level` switches with it: its own `g_flags & 1` replaces the
 * father price with the flat `1500 * difficulty + 2000` the intervention costs. One
 * counter, two currencies, selected by the same bit at both ends.
 *
 * The three bits of 0x7784 this reads are all latches. Bit 0 is independence declared,
 * bit 1 is the intervention already spent, and bit 2 is set here, by this function, the
 * first time the pot grows after declaring -- the one-shot AMBUSHHINT/CONSIDER briefing
 * that tells the player what the bells now pay for. `g_cur_nation_rec->bells > bells` is
 * the test that fires it: the pot is larger than this single payment, so it had a balance
 * already and this is not the first bell of the war.
 *
 * NAME: the Macintosh build's `add_liberty(short, short)`, on arity and role together.
 * Two shorts, a nation and an amount, and the Mac's other liberty symbols do not fit --
 * `give_me_liberty` takes three arguments and `liberty_declared` takes none.
 *
 * The callee at 1020:a926 keeps its address name. It fills a numbered text slot with an
 * integer, but every candidate spelling for what that number MEANS here is a guess, and
 * the placeholder is cheaper to correct than a wrong word.
 *
 * `g_cur_nation_rec` is declared `*` in this module and plain `*` in
 * set_europe.c: the shipped code reaches the POINTER through an explicit SEG34
 * segment immediate, where the declaration reaches it through DS.
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_europe(int n);
extern void popup_country(int slot, int flag, int nation);
extern int popm(char *msg, int n);

/* Near-called, so they live in this object; stubs are enough to place the calls. */
extern void choose_freedom(int nation);
extern int freedom_level(int nation);

extern void give_me_liberty(int nation, int father, int z);

void add_liberty(int nation, int bells)
{
    set_europe(nation);
    g_cur_nation_rec->bells += bells;
    g_cur_nation_rec->bells_total += bells;
    if (!(g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_cur_nation_rec->next_father < 0)
        choose_freedom(nation);
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && !(g_game.game_flags & GAMEFLAG_INTERVENTION) && !(g_game.game_flags & GAMEFLAG_ADVICE_GIVEN)
        && g_cur_nation_rec->bells > bells) {
        popup_country(0, 1, g_game.intervention_nation);
        set_text_arg_i_1020_a926(0, freedom_level(nation));
        popm("AMBUSHHINT", 1);
        popm("CONSIDER", 1);
        g_game.game_flags |= 4;
    }
    if (freedom_level(nation) <= g_cur_nation_rec->bells) {
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            if (!(g_game.game_flags & GAMEFLAG_INTERVENTION)) {
                intervention();
                g_cur_nation_rec->bells = 0;
            }
        } else {
            if (g_cur_nation_rec->next_father >= 0)
                give_me_liberty(nation, g_cur_nation_rec->next_father, 0);
            g_cur_nation_rec->bells = 0;
        }
    }
}
