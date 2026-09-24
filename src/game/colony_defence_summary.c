/* colony_defence_summary -- shipped at 1018:816b.
 *
 * Ported by tools/port.py from matched/game/colony_defence_summary.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 0xca bytes */



extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 tile_unit_tally(col_i16 u, col_i16 kind);

col_i16 colony_defence_summary(col_i16 *best_out, col_i16 *own_out, col_i16 *total_out, col_i16 nation)
{
    register col_i16 u;
    register col_i16 dir;
    col_i16 total;
    col_i16 best;
    col_i16 v;
    col_i16 bestv;
    col_i16 x;
    col_i16 y;
    col_i16 nat;
    col_i16 own;
    col_i16 sum;

    u = unit_at(g_cur_colony_rec->x, g_cur_colony_rec->y);
    if (u >= 0)
        own = tile_unit_tally(u, 10);
    else
        own = 0;
    best = -1;
    total = 0;
    sum = 0;
    bestv = 0;
    for (dir = 0; dir < 8; dir++) {
        x = g_cur_colony_rec->x + g_dir_dx[dir];
        y = g_cur_colony_rec->y + g_dir_dy[dir];
        u = unit_at(x, y);
        if (u >= 0) {
            nat = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
            v = tile_unit_tally(u, 10);
            if (nat != nation)
                continue;
            else {
                sum += tile_unit_tally(u, 11) >> 3;
                total += v;
                if (v >= bestv) {
                    bestv = v;
                    best = nat;
                }
            }
        }
    }
    if (best_out != 0)
        *best_out = best;
    if (own_out != 0)
        *own_out = own;
    if (total_out != 0)
        *total_out = total;
    return sum;
}
