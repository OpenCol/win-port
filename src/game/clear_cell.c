/* clear_cell -- shipped at 1028:0000.
 *
 * Ported by tools/port.py from matched/game/clear_cell.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Cell far g_cells[][64];                       SEG24:0x0000
 *
 * The file's own Cell dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f2 -> c, f3 -> d
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 4 bytes */


void clear_cell(col_i16 a, col_i16 b)
{
    g_cells[a][b].c = 0xff;
    g_cells[a][b].d = 0;
}
