/* arrived_unit_orders_0 -- shipped at 1008:008f.
 *
 * Ported by tools/port.py from matched/game/arrived_unit_orders_0.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
void arrived_unit_orders_0(void)
{
    g_units[g_selected_unit].orders = ORDERS_NONE;
}
