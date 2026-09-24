/* rebuild_cell_row -- shipped at 1028:05cc.
 *
 * Ported by tools/port.py from matched/game/rebuild_cell_row.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Cell far g_cells2[][16];                      SEG24:0x0400
 *
 * The file's own Cell dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void cell_note_or_insert_1028_0290(col_i16 row, col_i16 a, col_i16 b, col_i16 c, col_i16 d);
void clear_cell(col_i16 row, col_i16 i);

/* same object: 4-byte `push cs; call ` */



void rebuild_cell_row(col_i16 row)
{
    register col_i16 i;

    for (i = 0; i < 0x40; i++)
        clear_cell(row, i);
    for (i = 0; i < 0x10; i++) {
        if ((col_i16)g_cells2[row][i].c >= 0)
            cell_note_or_insert_1028_0290(row, g_cells2[row][i].a, g_cells2[row][i].b,
                                          g_cells2[row][i].c, g_cells2[row][i].d);
    }
}
