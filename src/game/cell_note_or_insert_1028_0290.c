/* cell_note_or_insert_1028_0290 -- shipped at 1028:0290.
 *
 * Ported by tools/port.py from matched/game/cell_note_or_insert_1028_0290.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Cell far g_cells[][0x40];                     SEG24:0x0000
 *
 * The file's own Cell dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void shift_cells_up(col_i16 row, col_i16 i);

/* same object: 4-byte `push cs; call ` (rule 47) */

void cell_note_or_insert_1028_0290(col_i16 row, char a, char b, char c, char d)
{
    register col_i16 i;

    for (i = 0; i < 0x40; i++) {
        if (g_cells[row][i].a == a && g_cells[row][i].b == b &&
            g_cells[row][i].c == c && g_cells[row][i].d >= d)
            goto done;
    }
    for (i = 0; i < 0x40; i++) {
        if (g_cells[row][i].d < d || (col_i16)g_cells[row][i].c == -1) {
            shift_cells_up(row, i);
            g_cells[row][i].a = a;
            g_cells[row][i].b = b;
            g_cells[row][i].c = c;
            g_cells[row][i].d = d;
            goto done;
        }
    }
done:
    ;
}
