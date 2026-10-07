/* get_america_name: candidate for 1030:8135 (79 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The 0x34-byte record at SEG20:0x7828 for nation n, except that when bit 0 of
 * SEG20:0x7784 is set and n is the nation at SEG20:0x77d4, the record of the
 * nation whose index is at SEG20:0x779a is substituted.
 *
 * RENAMED 2026-09-26 from tribe_record_for: Returns &g_nation_text[n] (0x34-byte European nation records at SEG20:0x7828, whose +0 is the nation's land name -- the same array land_ho, royal_edicts and EuropeWin_DrawInbound call g_nation_text), substituting the player's record for the mother country after independence. Its only caller uses it for nat < 4 and uses g_tribe_names for the natives; 'tribe' names the wrong subject (natives instead of European nations).
 *
 * RENAMED 2026-09-27 from nation_record_for: the Mac build's get_america_name, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Returns g_nation_text[n].country, the nation's New World land name, with the player's record substituted for the mother country after independence; StatWin_Stats STRCATs the result as a string, and StatWin::Stats is the Mac caller; 1 arg.
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

char *get_america_name(int n)
{
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && g_game.mother_country_idx == n)
        return g_nation_text[g_game.player_nation].country;
    return g_nation_text[n].country;
}
