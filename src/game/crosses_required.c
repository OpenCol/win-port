/* crosses_required -- shipped at 1048:7757.
 *
 * Ported by tools/port.py from matched/game/crosses_required.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *
 * The file's own Colony, Unit, Nation, NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1048_db44(col_i16 a, col_i16 b);

#include "nations.h"
#include "control.h"
/* 0xca bytes */
#define NATION(u) ((unsigned char)((u).flags & 0xf))
/* 0x13c bytes */
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */








extern col_i16 unit_colonist_class(col_i16 u);

/* same object: 4-byte `push cs; call ` */


col_i16 crosses_required(col_i16 nation, col_i16 *rate)
{
    register col_i16 i;
    register col_i16 total;

    *rate = 2;
    total = 0;
    for (i = 0; g_colony_count > i; i++) {
        if (g_colonies[i].nation == (unsigned char)nation)
            total = g_colonies[i].pop + total;
    }
    for (i = 0; g_unit_count > i; i++) {
        if (NATION(g_units[i]) == (unsigned char)nation) {
            total = total + 1;
            if (g_units[i].x == (unsigned char)(nation + 0xec)
                && unit_colonist_class(i) >= 0
                && (g_nations[nation].flags & 0x40)) {
                if (*rate > 0)
                    *rate = -2;
                else
                    *rate -= 2;
            }
        }
    }
    if (total < 0xfa0)
        total = total * 2;
    total += 8;
    total = min_int_1048_db44(total, 0xfa0);
    if (g_cur_nation >= NATION_EUROPEAN_COUNT || g_nation52[g_cur_nation].control != CONTROL_HUMAN)
        total = (8 - g_difficulty) * total >> 3;
    if (g_cur_nation == 0)
        total = total * 2 / 3;
    return total;
}
