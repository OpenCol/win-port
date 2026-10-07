/* globals_swap_words: candidate for 1008:8cdc (862 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Byte-order pass over the game's global state block in the data segment: 26 words
 * one by one, then three small arrays (4, 4+4 and 16 words) in loops. Every global is
 * ``-qualified, so each access loads its own segment.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

void globals_swap_words(void *unused1)
{
    int i;

    g_game.tutorial_flags = swap_word_identity(g_game.tutorial_flags); g_game.game_flags = swap_word_identity(g_game.game_flags); g_game.colony_options = swap_word_identity(g_game.colony_options);
    g_game.america_map = swap_word_identity(g_game.america_map); g_game.year = swap_word_identity(g_game.year); g_game.season = swap_word_identity(g_game.season);
    g_game.turn_counter = swap_word_identity(g_game.turn_counter); g_game.view_mode = swap_word_identity(g_game.view_mode); g_game.selected_unit = swap_word_identity(g_game.selected_unit);
    g_game.turn_nation = swap_word_identity(g_game.turn_nation); g_game.view_nation = swap_word_identity(g_game.view_nation); g_game.player_nation = swap_word_identity(g_game.player_nation);
    g_game.settlement_count = swap_word_identity(g_game.settlement_count); g_game.unit_count = swap_word_identity(g_game.unit_count); g_game.colony_count = swap_word_identity(g_game.colony_count);
    g_game.trade_route_count = swap_word_identity(g_game.trade_route_count); g_game.reveal_all = swap_word_identity(g_game.reveal_all); g_game.view_override = swap_word_identity(g_game.view_override);
    g_game.game_in_progress = swap_word_identity(g_game.game_in_progress); g_game.human_turn_active = swap_word_identity(g_game.human_turn_active); g_game.end_of_turn_pending = swap_word_identity(g_game.end_of_turn_pending);
    for (i = 0; i < 4; i++)
        g_game.war_started[i] = swap_word_identity(g_game.war_started[i]);
    g_game.rebel_sentiment = swap_word_identity(g_game.rebel_sentiment); g_game.mother_country_idx = swap_word_identity(g_game.mother_country_idx); g_game.intervention_nation = swap_word_identity(g_game.intervention_nation);
    g_game.merc_nation = swap_word_identity(g_game.merc_nation); g_game.rebel_step = swap_word_identity(g_game.rebel_step);
    for (i = 0; i < 4; i++) {
        g_game.royal_forces[i] = swap_word_identity(g_game.royal_forces[i]);
        g_game.force[i] = swap_word_identity(g_game.force[i]);
    }
    for (i = 0; i < 16; i++)
        g_game.base_sales[i] = swap_word_identity(g_game.base_sales[i]);
}
