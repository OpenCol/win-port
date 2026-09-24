/* set_slot_from_table -- shipped at 1018:4943.
 *
 * Ported by tools/port.py from matched/game/set_slot_from_table.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_prod[];                             SEG21:0x0122
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_slot_of(col_i16 k);
col_i16 current_colony_building_count(col_i16 b);
void set_slot_value(col_i16 slot, col_i16 v);

void set_slot_from_table(col_i16 slot, col_i16 k)
{
    register col_i16 v;
    col_i16 b;

    v = g_prod[k];
    b = building_slot_of(k);
    if (current_colony_building_count(b) > 2)
        v = v * 2 / 3;
    set_slot_value(slot, v);
}
