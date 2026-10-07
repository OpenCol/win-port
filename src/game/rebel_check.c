/* rebel_check: candidate for 1038:79e8 (474 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Recomputes a nation's rebel sentiment (rebel_level) into its record byte at +0x19 of the
 * 0x13c-byte nation records. For the player the value also goes to SEG20:0x77d2; during the
 * revolution only the tory-penalty routine runs (for the player or the mother country) and nothing is stored;
 * otherwise the King may mobilise, the War of Spanish Succession fires at 50% with no mother
 * country yet, and with a population of at least four (DGROUP:0x4b54) each new ten-percent step up
 * (REBELUP50 past 50%, REBELUP below) or down (REBELDOWN) is announced and remembered
 * at SEG20:0x77da.
 *
 * RENAMED 2026-09-26 from update_rebel_sentiment: the Mac build's rebel_check(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller new_turn_bookkeeping; callee conquest; callee force_mood; callee popm; callee popup_country), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

extern void revolution_check(int nation);
extern void royal_edicts(int nation);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int rebel_level(int nation);
extern void conquest(void);
extern void force_mood(int kind);

extern void popup_country(int slot, int flag, int nation);
extern int popm(char *msg, int n);

void rebel_check(int nation)
{
    int s;
    int player;

    player = g_game.player_nation == nation || ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_game.mother_country_idx == nation);
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
        if (player)
            revolution_check(nation);
        goto done;
    }
    if (player)
        royal_edicts(nation);
    s = rebel_level(nation);
    g_nations[nation].support = s;
    if (!player)
        goto done;
    g_game.rebel_sentiment = s;
    if (g_game.rebel_sentiment >= 0x32 && g_game.mother_country_idx < 0)
        conquest();
    if (g_nation_population[nation] >= 4) {
        if (g_game.rebel_sentiment / 10 > g_game.rebel_step) {
            force_mood(3);
            set_text_arg_i_1038_86cc(0, g_game.rebel_sentiment);
            popup_country(0, 0, g_game.player_nation);
            if (g_game.rebel_sentiment >= 0x32)
                popm("REBELUP50", 1);
            else
                popm("REBELUP", 1);
            g_game.rebel_step = g_game.rebel_sentiment / 10;
        } else if ((g_game.rebel_sentiment + 4) / 10 < g_game.rebel_step) {
            force_mood(1);
            set_text_arg_i_1038_86cc(0, g_game.rebel_sentiment);
            popup_country(0, 0, g_game.player_nation);
            popm("REBELDOWN", 1);
            g_game.rebel_step = g_game.rebel_sentiment / 10;
        }
    }
done:
    ;
}
