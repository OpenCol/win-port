/* europe_new_unit -- shipped at 1048:4b3e.
 *
 * Ported by tools/port.py from matched/game/europe_new_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "control.h"
#include "nations.h"
#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */



extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern col_i16 create_unit_1038_2246(col_i16 type, col_i16 nation, col_i16 x, col_i16 y);

col_i16 europe_new_unit(col_i16 code)
{
    col_i16 diff;
    register col_i16 type;
    register col_i16 u;

    type = 0;
    if (code == 0x14)
        type = 2;
    if (code == 0x18)
        type = 3;
    if (code == 0x16)
        type = 5;
    if (code == 0x15) {
        type = 1;
        diff = (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN)
               ? g_difficulty : 1;
        if (rand_range(0, diff + 4) == 0)
            type = 4;
    }
    u = create_unit_1038_2246(type, g_cur_nation, g_cur_nation - 20, g_cur_nation - 20);
    if (u < 0)
        goto done;
    g_units[u].orders = ORDERS_SENTRY;
    g_units[u].spec = (unsigned char)code;
    if (type == 2)
        ((unsigned char *)((unsigned char *)&(g_units[u]) + 0x10))[5] = 0x64;
done:
    return u;
}
