/* get_adjective: candidate for 1030:8033 (124 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A per-nation word: tribes (index 4 and up) read the 6-byte table at SEG20:0xd3f2, 0x16
 * bytes before the indexed entry; nations read the word table at SEG20:0xd89e, unless bit 0
 * of the byte at 0x7784 is set and the index is the PLAYER (0x779a) or the MOTHER
 * COUNTRY (0x77d4), whose words are replaced by 0xdd52 and 0xdd54. That is the post-
 * independence renaming: once war is declared the two sides stop being called what they were.
 *
 * RENAMED 2026-09-26 from lookup_with_two_overrides: the Mac build's get_name(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller Stats; caller combat_fight; caller indian_move; caller indian_turn; caller resolve_raid), the runner-up gets 8, the arity agrees (1), and the Mac body is 0.87x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says. The body is a per-nation name lookup with the post-independence renaming of the player and the mother country.
 *
 * RENAMED 2026-09-27 from get_name: This is the other half of the 7fb7 swap. Call graph: 32 of Mac get_adjective(short)'s 34 callers are among 8033's 39 callers, against 9 of Mac get_name's 15. They include piss_off (1038:02a1), tribe_hostile (c850), tribe_burn_missions, ReportsForeign, allied_invasion, colony_fortress, delete_village, end_of_game_check, europe_new_wars, foreign_scout, free_galleon2, intervention, new_turn_bookkeeping, orders_clear/road, plan_move, process_off_board_ships, special_kill, top_unit_in_square, tribe_camp/heresy/mission/village and try_to_set_job. Layout: it is second of the pair on both sides, directly before get_true_name. The body agrees: it reads 0xd3f2-0x16, i.e. +2 of the SEG20:0xd3da tribe record, which is the singular or adjective column ('Inca'). Why it went wrong: tools/mac_callgraph.py bound it with a margin of 9 edges against 8 (its RENAMED line says 'The margin is small'), and the body check could not tell a noun lookup from an adjective lookup.
 */

#include "colonization.h"

#include "gameflag.h"
#include "misc.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

int get_adjective(int nation)
{
    if (nation >= 4)
        return ((int *)((char *)&g_tribe[nation] - 0x16))[0];
    else {
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            if (g_game.player_nation == nation)
                return g_misc_words[MISC_REBEL];
            if (g_game.mother_country_idx == nation)
                return g_misc_words[MISC_TORY];
        }
        return g_nation_adjective[nation];
    }
}
