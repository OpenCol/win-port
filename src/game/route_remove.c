/* route_remove: candidate for 1028:c70e (272 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Removes trade route `idx` from the table: units on it are taken off and their "following
 * a route" order cleared, units on a higher route are renumbered down, then the 0x4a-byte
 * records compact by one struct copy each and the count comes down.
 *
 * THE BINARY CONTAINS THIS LOOP TWICE. `route_delete` at 1028:d088 -- the dialog, with
 * `TRADEDELETE` and `SUREDELETE` in front of it -- has the whole body inline again, and the
 * two copies do not agree:
 *
 *   - this one tests the unit's nation first, `(flags & 0xf) == g_game.turn_nation`; the
 *     dialog's copy renumbers every unit in the game.
 *   - for a unit on a HIGHER route, the dialog calls `set_unit_route(u, i - 1)` and this
 *     one calls `set_unit_dest(u, r - 1)`.
 *
 * The second difference looks like a bug. `get_unit_route`/`set_unit_route` are the route
 * index and `get_unit_dest`/`set_unit_dest` are the stop index --
 * `kill_dest` uses the flag pair for the stop throughout, and this function's own
 * `set_unit_route(u, 0)` and `set_unit_dest(u, 0)` for the deleted route set both. So
 * the renumber here writes the decremented ROUTE index into the STOP field. I have not
 * found which of the two is reachable in play; the bytes are what they are and are left
 * alone.
 *
 * `F_SCOPY` with `CX = 0x4a` is the struct assignment `g_routes[i] = g_routes[i + 1]`.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int get_unit_route(int u);
extern void set_unit_route(int u, int r);
extern void set_unit_dest(int u, int f);

void route_remove(int idx)
{
    int r;
    register int u;
    register int i;

    for (u = 0; u < g_game.unit_count; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) == ((unsigned char)g_game.turn_nation)) {
            if (g_unit_types[g_units[u].type].holds > 0) {
                r = get_unit_route(u);
                if (idx == r) {
                    set_unit_route(u, 0);
                    set_unit_dest(u, 0);
                    if (g_units[u].orders == ORDERS_TRADE_ROUTE)
                        g_units[u].orders = ORDERS_NONE;
                } else if (r > idx) {
                    set_unit_dest(u, r - 1);
                }
            }
        }
    }
    for (i = idx; g_game.trade_route_count - 1 > i; i++)
        g_routes[i] = g_routes[i + 1];
    g_game.trade_route_count--;
}
