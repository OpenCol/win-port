/* get_adjective -- shipped at 1030:7fb7.
 *
 * Ported by tools/port.py from matched/game/get_adjective.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern TribeName far g_tribe_names[];                SEG20:0xd3da
 *   extern int far g_nation_adjective[];                 SEG20:0xd89e
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *   extern int far g_rebel_adjective;                    SEG20:0xdd74
 *   extern int far g_royal_adjective;                    SEG20:0xdd76
 *
 * The file's own TribeName dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "gameflag.h"
/* 6 bytes at SEG20:0xd3da, four
                    records before the tribes' own start at 0xd3f2, so indexing it by the nation
                    number lands on tribe n's record. THIS READS +0, THE PLURAL NOUN. The TRIBES
                    resource is "Incas, Inca, Jewelled Relics" and `load_data_tables` stores the
                    columns in order, so +0 is "Incas" and +2 "Inca" -- which is what
                    `nation_text_field2` returns. Both functions fall back to the European
                    adjective table for nations under 4, so the asymmetry is the game's: a tribe
                    contributes its plural noun where a European contributes an adjective. */








col_i16 get_adjective(col_i16 n)
{
    if (n >= 4)
        return g_tribe_names[n].name;
    else {
        if ((*g_game_flags) & GAMEFLAG_INDEPENDENCE) {
            if (g_player_nation == n)
                return g_rebel_adjective;
            if (g_mother_country_idx == n)
                return g_royal_adjective;
        }
        return g_nation_adjective[n];
    }
}
