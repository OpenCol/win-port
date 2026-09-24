/* unit_replace_at_self -- shipped at 1038:1dbb.
 *
 * Ported by tools/port.py from matched/game/unit_replace_at_self.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_detach_attach(col_i16 unit, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */

void unit_replace_at_self(col_i16 unit)
{
    col_i16 n = unit;

    unit_detach_attach(n, g_units[n].x, g_units[n].y);
}
