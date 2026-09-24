/* clear_cells_near -- shipped at 1028:0042.
 *
 * Ported by tools/port.py from matched/game/clear_cells_near.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Cell far g_cells2[][16];                      SEG24:0x0400
 *
 * The file's own Cell dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 octile_distance_xy(col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);

void clear_cells_near(col_i16 row, char kind, col_i16 x, col_i16 y, col_i16 radius)
{
    col_i16 d;
    register col_i16 i;

    for (i = 0; i < 0x10; i++) {
        if (g_cells2[row][i].c == kind) {
            d = octile_distance_xy(x, y, g_cells2[row][i].a, g_cells2[row][i].b);
            if (d <= radius) {
                g_cells2[row][i].c = 0xff;
                g_cells2[row][i].d = 0;
            }
        }
    }
}
