/* unit_clear_flags_high -- shipped at 1038:270a.
 *
 * Ported by tools/port.py from matched/game/unit_clear_flags_high.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void unit_clear_flags_high(col_i16 u)
{
    if (u >= 0)
        g_units[u].flags &= 0xf;
}
