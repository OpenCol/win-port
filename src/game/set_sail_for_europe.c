/* set_sail_for_europe -- shipped at 1030:2ede.
 *
 * Ported by tools/port.py from matched/game/set_sail_for_europe.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern unsigned char far g_land_units[];             DGROUP:0x4b74
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 arrive_counter_start(col_i16 u, col_i16 a, col_i16 b);

#include "units.h"
extern void ship_take_aboard(col_i16 u);
extern col_i16 unit_chain_tail(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);
extern void unit_chain_relink(col_i16 u, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` (rule 47) */


void set_sail_for_europe(register col_i16 unit)
{
    col_i16 turns;
    register col_i16 u;

    ship_take_aboard(unit);
    turns = arrive_counter_start(unit, g_units[unit].dest_x, g_units[unit].dest_y);
    u = unit_chain_tail(unit);
    while (u >= 0) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            g_land_units[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]--;
        g_units[u].counter = turns;
        u = unit_chain_next(u);
    }
    unit_chain_relink(unit, (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK) - 0x18,
                                (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK) - 0x18);
}
