/* nation_sol_average -- shipped at 1038:4703.
 *
 * Ported by tools/port.py from matched/game/nation_sol_average.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void set_current_colony(col_i16 c);
extern col_i16 colony_sol_percent(void);

col_i16 nation_sol_average(col_i16 nation)
{
    register col_i16 c;
    col_i32 pop = 0;
    col_i32 sum = 0;

    for (c = 0; c < g_colony_count; c++) {
        set_current_colony(c);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        pop += (col_i32)g_cur_colony_rec->pop;
        sum += (col_i32)colony_sol_percent() * (col_i32)g_cur_colony_rec->pop;
    }
    if (pop != 0)
        sum = sum / pop;
    return (col_i16)sum;
}
