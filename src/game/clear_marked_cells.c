/* clear_marked_cells -- shipped at 1018:72e4.
 *
 * Ported by tools/port.py from matched/game/clear_marked_cells.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_grid_marks[5][5];                  SEG21:0x014a
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_set_cell(col_i16 i, col_i16 j, col_i16 v);

void clear_marked_cells(void)
{
    register col_i16 j;
    register col_i16 i;

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++) {
            if (g_grid_marks[i][j] != 0)
                colony_set_cell(i, j, -1);
        }
    }
}
