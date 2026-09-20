/* tribe_record_for -- shipped at 1030:8135.
 *
 * Ported by tools/port.py from matched/game/tribe_record_for.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec far g_rec52[];                      SEG20:0x7828
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_player_nation;                      SEG20:0x779a
 *
 * The file's own NationRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "gameflag.h"
NationRec * tribe_record_for(col_i16 n)
{
    if (((*g_game_flags) & GAMEFLAG_INDEPENDENCE) && g_mother_country_idx == n)
        return &g_rec52[g_player_nation];
    return &g_rec52[n];
}
