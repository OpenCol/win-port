/* scaled_rate -- shipped at 1048:4f66.
 *
 * Ported by tools/port.py from matched/game/scaled_rate.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "control.h"
#include "nations.h"
/* +1 is CONTROL: 0 human, 1 computer, 2 defeated */




col_i16 scaled_rate(col_i16 v)
{
    register col_i16 r;

    r = (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN) ? g_difficulty - 2 : -2;
    r = r << 4;
    r = r * v / 100;
    return r;
}
