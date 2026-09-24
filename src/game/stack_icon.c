/* stack_icon -- shipped at 1040:0b6c.
 *
 * Ported by tools/port.py from matched/game/stack_icon.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 14 bytes at SEG20:0x7ac8 */


extern col_i16 colony_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_tail(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);

col_i16 stack_icon(register col_i16 u, col_i16 own, col_i16 self, col_i16 *more)
{
    col_i16 ship;
    col_i16 icon;
    col_i16 tail;
    col_i16 col;
    register col_i16 unit;

    ship = -1;
    icon = -1;
    if (own == 0 || self == 0) {
        col = colony_at(g_units[u].x, g_units[u].y);
        if (col >= 0)
            goto out;
    }
    tail = unit_chain_tail(u);
    *more = unit_chain_next(tail) >= 0;
    if (own == 0) {
        u = tail;
    } else if (self != 0) {
        icon = g_unit_types[g_units[u].type].icon;
        goto out;
    }
    for (unit = tail; unit >= 0; unit = unit_chain_next(unit))
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_FRIGATE)
            ship = unit;
    if (own != 0) {
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_FRIGATE)
            goto out;
        if (ship < 0)
            goto out;
    }
    if (ship >= 0)
        icon = g_unit_types[g_units[ship].type].icon;
    else
        icon = g_unit_types[g_units[u].type].icon;
out:
    return icon;
}
