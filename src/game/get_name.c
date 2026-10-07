/* get_name: candidate for 1030:7fb7 (124 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * name-source: sibling-build
 *
 * The adjective word for a nation: tribes (4 and up) from the 6-byte tribe-name records at
 * SEG20:0xd3da; after the declaration (SEG20:0x7784 bit 0) the player's nation is the
 * rebels' word (SEG20:0xdd74) and the mother country the royalists' (0xdd76); otherwise the
 * word table at SEG20:0xd89e.
 *
 * RENAMED 2026-09-27 from get_adjective: This is not in the witness list; I found it while checking piss_off, whose Mac get_adjective calls land on the Windows get_name. Call graph: all 15 Mac callers of get_name(short) are among 7fb7's 19 callers (CheatMenuBlow, ReportWin::ReportsIndian, StatWin::Stats, combat_fight, conquest, indian_move, indian_turn, move_unit, orders_fortify, resolve_raid, tribe_burn_missions, tribe_extortion, tribe_incite, tribe_live, tribe_village). Only 9 of Mac get_adjective's 34 callers are. Layout: Mac CODE 15 runs treaty_off, get_name (0x666c), get_adjective (0x66d8), get_true_name, and Windows 1030 runs treaty_off (7f43), 7fb7, 8033, get_true_name (80b5). The body agrees: 7fb7 reads +0 of the 6-byte tribe record, which is the plural noun ('Incas', per its own header), and falls back to the nation word table. Arity: (short) against (int nation). The sizes (Mac 108/112 against 124/124) cannot tell the two apart. This is a swap with 1030:8033 and needs a temporary name.
 */

#include "colonization.h"

#include "gameflag.h"
#include "misc.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

int get_name(int nation)
{
    if (nation >= 4)
        return g_tribe[nation - 4].name;
    else {
        if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
            if (g_game.player_nation == nation)
                return g_misc_words[MISC_REBELS];
            if (g_game.mother_country_idx == nation)
                return g_misc_words[MISC_TORIES];
        }
        return g_nation_adjective[nation];
    }
}
