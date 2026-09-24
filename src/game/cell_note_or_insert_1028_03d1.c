/* cell_note_or_insert_1028_03d1 -- shipped at 1028:03d1.
 *
 * Ported by tools/port.py from matched/game/cell_note_or_insert_1028_03d1.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Cell far g_cells2[][0x10];                    SEG24:0x0400
 *
 * The file's own Cell dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void shift_cells2_up(col_i16 row, col_i16 i);

/* same object: 4-byte `push cs; call ` (rule 47) */

void cell_note_or_insert_1028_03d1(col_i16 row, char a, char b, char c, char d)
{
    register col_i16 i;

    for (i = 0; i < 0x10; i++) {
        if (g_cells2[row][i].a == a && g_cells2[row][i].b == b &&
            g_cells2[row][i].c == c && g_cells2[row][i].d >= d)
            goto done;
    }
    for (i = 0; i < 0x10; i++) {
        if (g_cells2[row][i].d < d || (col_i16)g_cells2[row][i].c == -1) {
            shift_cells2_up(row, i);
            g_cells2[row][i].a = a;
            g_cells2[row][i].b = b;
            g_cells2[row][i].c = c;
            g_cells2[row][i].d = d;
            goto done;
        }
    }
done:
    ;
}
