/* unit_drop_cargo_slot -- shipped at 1018:7e18.
 *
 * Ported by tools/port.py from matched/game/unit_drop_cargo_slot.c.
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
col_i16 unit_set_cargo_type(col_i16 u, col_i16 slot, col_i16 type);
void unit_set_cargo_amount(col_i16 u, col_i16 slot, col_i16 amt);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */











col_i16 unit_drop_cargo_slot(col_i16 u, col_i16 slot)
{
    register col_i16 k;
    col_i16 t;

    t = unit_cargo_type(u, slot);
    if (t < 0)
        goto done;
    g_cargo_amount = unit_cargo_amount(u, slot);
    for (k = slot; k < g_units[u].cargo_count - 1; k++) {
        unit_set_cargo_type(u, k, unit_cargo_type(u, k + 1));
        unit_set_cargo_amount(u, k, unit_cargo_amount(u, k + 1));
    }
    g_units[u].cargo_count--;
done:
    return t;
}
