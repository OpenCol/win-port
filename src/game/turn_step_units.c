/* turn_step_units -- shipped at 1008:2b6d.
 *
 * Ported by tools/port.py from matched/game/turn_step_units.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern char far g_loaded_flag;                       DGROUP:0x008b
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_view_nation;                        SEG20:0x7798
 *   extern int far g_view_override;                      SEG20:0x77a6
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void rank_nations(void);
extern void reset_tribe_claims(void);

void turn_step_units(void *ctx)
{
    col_i16 i;

    if (g_loaded_flag == 0) {
        rank_nations();
        for (i = 0; g_unit_count > i; i++)
            g_units[i].moves = 0;
        g_view_nation = g_player_nation;
        if (g_view_override >= 0)
            g_view_nation = g_view_override;
        reset_tribe_claims();
    }
}
