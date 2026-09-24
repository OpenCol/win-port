/* init_grid_marks -- shipped at 1018:70cd.
 *
 * Ported by tools/port.py from matched/game/init_grid_marks.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_grid_marks[5][5];                  SEG21:0x014a
 *   extern char g_grid_marks_done;                       DGROUP:0x0c22
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char colony_tile_flags(col_i16 i, col_i16 j);

void init_grid_marks(void)
{
    register col_i16 j;
    register col_i16 i;

    if (g_grid_marks_done != 0)
        goto done;
    for (j = 0; j < 5; j++) {
        for (i = 0; i < 5; i++)
            g_grid_marks[i][j] = colony_tile_flags(i, j);
    }
done:
    g_grid_marks_done = 1;
}
