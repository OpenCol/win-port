/* find_colony_grid_cell -- shipped at 1018:627f.
 *
 * Ported by tools/port.py from matched/game/find_colony_grid_cell.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char colony_cell_colonist(col_i16 i, col_i16 j);

col_i16 find_colony_grid_cell(char v, col_i16 *px, col_i16 *py)
{
    col_i16 found = 0;
    register col_i16 j;
    register col_i16 i;

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++) {
            if (colony_cell_colonist(i, j) == v) {
                *px = i;
                *py = j;
                found = 1;
            }
        }
    }
    return found;
}
