/* lookup_with_two_overrides -- shipped at 1030:8033.
 *
 * Ported by tools/port.py from matched/game/lookup_with_two_overrides.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern TribeName far g_tribe[];                      SEG20:0xd3f2
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_override_a_val;                     SEG20:0xdd52
 *   extern int far g_override_b_val;                     SEG20:0xdd54
 *   extern int far g_nation_adjective[];                 SEG20:0xd89e
 *
 * The file's own TribeName dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "gameflag.h"
col_i16 lookup_with_two_overrides(col_i16 n)
{
    if (n >= 4)
        return ((col_i16 *)((char *)&g_tribe[n] - 0x16))[0];
    else {
        if (g_game_flags & GAMEFLAG_INDEPENDENCE) {
            if (g_player_nation == n)
                return g_override_a_val;
            if (g_mother_country_idx == n)
                return g_override_b_val;
        }
        return g_nation_adjective[n];
    }
}
