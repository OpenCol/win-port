/* unit_find_cargo_slot -- shipped at 1018:7f78.
 *
 * Ported by tools/port.py from matched/game/unit_find_cargo_slot.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_cargo_amount;                           DGROUP:0x4bd2
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_cargo_amount(col_i16 u, col_i16 slot);
col_i16 unit_cargo_type(col_i16 u, col_i16 slot);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */







col_i16 unit_find_cargo_slot(col_i16 u, col_i16 goods)
{
    register col_i16 found = -1;
    register col_i16 i;
    col_i16 t;

    for (i = 0; found < 0 && i < g_units[u].cargo_count; i++) {
        t = unit_cargo_type(u, i);
        if (t == goods)
            found = i;
    }
    if (found >= 0)
        g_cargo_amount = unit_cargo_amount(u, found);
    return found;
}
