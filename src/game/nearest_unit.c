/* nearest_unit -- shipped at 1038:3b4e.
 *
 * Ported by tools/port.py from matched/game/nearest_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int g_nearest_unit_dist;                      DGROUP:0x4c38
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */



extern col_i16 octile_distance(col_i16 dx, col_i16 dy);

col_i16 nearest_unit(unsigned char nation, col_i16 skip, col_i16 x, col_i16 y)
{
    col_i16 best;
    col_i16 bestd;
    register col_i16 u;
    register col_i16 d;

    best = -1;
    bestd = 0x270f;
    for (u = 0; g_unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != nation)
            continue;
        if (u == skip)
            continue;
        d = octile_distance(x - g_units[u].x, y - g_units[u].y);
        if (d <= bestd) {
            best = u;
            bestd = d;
        }
    }
    g_nearest_unit_dist = bestd;
    return best;
}
