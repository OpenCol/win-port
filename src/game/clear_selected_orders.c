/* clear_selected_orders: candidate for 1008:008f (38 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the orders byte (Unit+8) of the unit at SEG20:0x7794 to 0.
 *
 * RENAMED 2026-09-26 from arrived_unit_orders_0: sets the selected unit's orders to ORDERS_NONE.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

void clear_selected_orders(void)
{
    g_units[g_game.selected_unit].orders = ORDERS_NONE;
}
