/* orders_sentry: candidate for 1008:0252 (38 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets the orders byte (Unit+8) of the unit at SEG20:0x7794 to 1.
 *
 * RENAMED 2026-09-26 from arrived_unit_orders_1: sets the selected unit's orders to ORDERS_SENTRY; GameBrains_KeyboardEvent's
 * sentry key.
 *
 * RENAMED 2026-09-26 from sentry_selected_unit: the Mac build's orders_sentry(void), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in GameBrains_KeyboardEvent -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (0).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

void orders_sentry(void)
{
    g_units[g_game.selected_unit].orders = ORDERS_SENTRY;
}
