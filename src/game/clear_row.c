/* clear_row -- shipped at 1028:0684.
 *
 * Ported by tools/port.py from matched/game/clear_row.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Cell far g_cells2[][16];                      SEG24:0x0400
 *
 * The file's own Cell dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: f2 -> c, f3 -> d
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 4 bytes */


void clear_row(col_i16 a)
{
    register col_i16 i;

    for (i = 0; i < 16; i++) {
        g_cells2[a][i].c = 0xff;
        g_cells2[a][i].d = 0;
    }
}
