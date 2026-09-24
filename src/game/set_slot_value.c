/* set_slot_value -- shipped at 1018:48e9.
 *
 * Ported by tools/port.py from matched/game/set_slot_value.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_prod[];                             SEG21:0x0122
 *   extern int far g_slot14_adjust;                      SEG21:0x01c0
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void set_value_and_overflow(col_i16 n, col_i16 v, col_i16 b, col_i16 w);

/* same object: 4-byte `push cs; call ` (rule 47) */

void set_slot_value(col_i16 idx, col_i16 b)
{
    col_i16 n = idx;
    col_i16 v = g_prod[n];

    if (n == 14 && g_slot14_adjust != 0)
        v -= g_slot14_adjust;
    set_value_and_overflow(n, v, b, ((col_i16 *)((unsigned char *)(g_cur_colony_rec) + 0x9a))[n]);
}
