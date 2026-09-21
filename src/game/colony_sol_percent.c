/* colony_sol_percent -- shipped at 1018:3c91.
 *
 * Ported by tools/port.py from matched/game/colony_sol_percent.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 min_int_1018_b329(col_i16 a, col_i16 b);

#include "fathers.h"
/* Colony+0xc6 -- the accumulated bells the SoL percentage divides into:
      `colony_sol_percent` computes `bells * 100 / this`, and `colony_end_of_turn`
      decays it by a 64th each turn and adds twice the population. Five files had
      called it bells_cap or bells_max after that denominator role */ /* 0xca bytes */

extern col_i16 nation_flag_test(col_i16 nation, col_i16 bit);



col_i16 colony_sol_percent(void)
{
    col_i32 v = 0;
    col_i32 bells = g_cur_colony_rec->bells;
    register col_i16 pct;

    if (g_cur_colony_rec->bells_cap > 0)
        v = bells * 100 / g_cur_colony_rec->bells_cap;
    pct = (col_i16)v;
    if (nation_flag_test(g_cur_colony_rec->nation, FATHER_SIMON_BOLIVAR) != 0)
        pct += 20;
    pct = min_int_1018_b329(100, pct);
    return pct;
}
