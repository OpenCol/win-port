/* unit_move_dir -- shipped at 1020:dfb7.
 *
 * Ported by tools/port.py from matched/game/unit_move_dir.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_move_to_square(col_i16 u, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */


void unit_move_dir(col_i16 u, col_i16 d)
{
    if (d == -1)
        d = 8;
    unit_move_to_square(u, g_units[u].x + g_dir_dx[d], g_units[u].y + g_dir_dy[d]);
}
