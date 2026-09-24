/* find_nearest_colony -- shipped at 1018:3a9e.
 *
 * Ported by tools/port.py from matched/game/find_nearest_colony.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern int g_nearest_dist;                           DGROUP:0x4bc6
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_current_colony(col_i16 c);

/* 0xca bytes */




extern col_i16 square_region_if_land(col_i16 x, col_i16 y);
extern col_i16 octile_distance(col_i16 dx, col_i16 dy);



col_i16 find_nearest_colony(col_i16 x, col_i16 y, col_i16 nation, col_i16 land)
{
    register col_i16 c;
    col_i16 d;
    col_i16 best = -1;
    col_i16 bestd = 9999;
    col_i16 nib;
    col_i16 check = 0;

    if (land == -2) {
        check = 1;
        land = square_region_if_land(x, y);
    }
    for (c = 0; c < g_colony_count; c++) {
        if (nation >= 0 && g_colonies[c].nation != (unsigned char)nation)
            continue;
        if (land >= 0) {
            nib = square_region_if_land(g_colonies[c].x, g_colonies[c].y);
            if (nib != land)
                continue;
        }
        if (check != 0 && !(g_colonies[c].status & 0x40))
            continue;
        d = octile_distance(x - g_colonies[c].x, y - g_colonies[c].y);
        if (d <= bestd) {
            best = c;
            bestd = d;
        }
    }
    g_nearest_dist = bestd;
    set_current_colony(best);
    return best;
}
