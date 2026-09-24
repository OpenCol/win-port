/* unit_outranks_f5 -- shipped at 1038:38ec.
 *
 * Ported by tools/port.py from matched/game/unit_outranks_f5.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_turn_nation;              SEG20:0x7796
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
unsigned char unit_move_points(col_i16 i);

#include "units.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

#define NATION(u) ((unsigned char)((u).flags & 0xf))

col_i16 unit_outranks_f5(col_i16 unit)
{
    col_i16 n = unit;
    col_i16 r = 0;

    if (n < 0 || g_unit_count <= n)
        goto done;
    if ((col_i16)g_units[n].x < 0)
        goto done;
    r = NATION(g_units[n]) == g_turn_nation
        && ((g_units[n].f4 & 0x80) == 0 || g_units[n].type == UNIT_ARTILLERY)
        && unit_move_points(n) > g_units[n].moves;
done:
    return r;
}
