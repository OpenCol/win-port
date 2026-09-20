/* score_from_difficulty_and_era -- shipped at 1020:a5c0.
 *
 * Ported by tools/port.py from matched/game/score_from_difficulty_and_era.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Rec316 far g_rec316[];                        SEG20:0xc2f4
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int far g_year;                               SEG20:0x778c
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *
 * The file's own NationRec52, Rec316 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "nations.h"
#include "control.h"
/* +1 is CONTROL: 0 human, 1 computer, 2 defeated */
col_i16 score_from_difficulty_and_era(col_i16 nation)
{
    col_i16 n = nation;
    col_i16 r;
    col_i16 v;

    v = (n < NATION_EUROPEAN_COUNT && g_nation52[n].control == CONTROL_HUMAN) ? g_difficulty * 2 + 6 : 14 - g_difficulty;
    v = v << 3;
    if (g_year >= 1600) v += v >> 1;
    if (g_year >= 1650) v += v >> 1;
    if (g_year >= 1700) v += v >> 1;
    if (g_year >= 1750) v += v >> 1;
    r = (g_rec316[n].level + 1) * v + 1;
    if (g_rec316[n].level == 0)
        r = r >> 1;
    if ((*g_game_flags) & 1)
        r = g_difficulty * 0x5dc + 0x7d0;
    return r;
}
