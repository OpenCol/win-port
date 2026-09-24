/* unit_set_orders_6 -- shipped at 1028:b1cc.
 *
 * Ported by tools/port.py from matched/game/unit_set_orders_6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
extern void unit_refresh_move_points(col_i16 unit);

void unit_set_orders_6(col_i16 unit)
{
    col_i16 n = unit;

    g_units[n].orders = ORDERS_FORTIFIED;
    unit_refresh_move_points(n);
}
