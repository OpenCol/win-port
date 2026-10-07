/* king_welcome: candidate for 1048:6b6f (82 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * The King's welcome: a word chosen by difficulty into the message, the player's nation
 * name into dialog option 1, and the welcome text resource asked.
 *
 * The nation index is `g_cur_nation` at DGROUP:0x4cc8, not `g_game.player_nation` at
 * SEG20:0x779a -- corrected 2026-09-20 from the shipped immediate, which
 * tools/check_global_addresses.py compares against the declared address. The two are
 * both "which nation" and the byte match cannot separate them, because the offset is a
 * masked fixup.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern void popup_say(int slot, int word);
extern void set_name_slot_1048_de36(int i, char *s);
/* same object: 4-byte `push cs; call ` (rule 47) */

void king_welcome(void)
{
    popup_say(0, g_difficulty_words[g_game.difficulty]);
    set_name_slot_1048_de36(1, g_nation_text[g_cur_nation].leader);
    ask_text_resource("KINGWELCOME0");
}
