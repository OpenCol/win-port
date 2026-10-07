/* stack_ship: candidate for 1038:3296 (701 bytes).  Verdict: RELOCATABLE.
 * symbol: stack_ship
 * name-source: sibling-build
 *
 * Takes onto a ship the units of its chain that should be aboard, while its holds last. A unit is
 * taken when its orders are ORDERS_SENTRY -- the game's own menu calls that order "Sentry / Board
 * ship" -- or when it is not sentried but stands in the WATER (`ocean`), or is off the map
 * anywhere other than its own nation's Europe (x == UNIT_EUROPE_X + nation). Each unit taken costs
 * its type's hold size (UnitType+8) out of the room the ship has left -- holds (+7) minus
 * cargo (Unit+0xc) -- is set to ORDERS_SENTRY, and is re-linked with `relocate_unit(u, -2, -2)`.
 * Units are walked from the chain's tail, skipping the ship itself.
 *
 * A ship ON A TRADE ROUTE gets no room at all, UNLESS it is out on the map on a square no colony
 * or settlement holds. Then the ship is lifted off its square (-4, -4), `find_orphan_cargo_units`
 * boards what is left there, the ship is put back, and a non-negative answer restores the room.
 * That step is skipped on a re-entrant call: DGROUP:0x193e is set for the duration and
 * `find_orphan_cargo_units` itself calls this function for every ship on a square.
 *
 * NAMING THE NUMBERS DID THE READING. As `orders == 1`, `orders == 2` and `x == nation + 0xec`
 * this was an unreadable filter; the UNITOPTIONS text ("Clear orders.", "Sentry / Board ship.",
 * "Fortify.") gave 0, 1 and 5 their names, and the orders values are now in include/units.h.
 * The Europe test inside the loop is dead in effect -- it can only set `take` for a sentried unit,
 * and a sentried unit was already taken.
 *
 * `idle = !take` is the shipped `NEG / SBB / INC`; `take == 0` compiles to a compare and jump.
 * `(int)(signed char)x < 0` is the `CBW` before the test (rule 51). 1038:3553 follows directly.
 *
 * RENAMED 2026-09-26 from ship_take_aboard: the Mac build's stack_ship(short), bound by call-graph agreement (tools/mac_callgraph.py): 9 of its Mac call edges already bound land on this function (caller attempt_to_set_sail; caller negotiate_europe; caller orders_disband; caller plan_move; callee is_city), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int is_city(int x, int y);
extern int terrain_is_ocean(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void sort_stack_by_class(int u);
extern void relocate_unit(int u, int x, int y);
extern int top_unit_in_square(int x, int y);
extern int find_orphan_cargo_units(int u);
extern int top_unit(int u);
extern int next_unit(int u);

void stack_ship(int ship)
{
    int u;
    int next;
    int room;
    int size;
    int take;
    int idle;
    int ok;
    int saved;
    int r;
    int sx;
    int sy;
    register int s;

    s = ship;
    room = 0;
    ok = 1;
    saved = g_boarding_in_progress;
    g_boarding_in_progress = 1;
    if (saved == 0) {
        sort_stack_by_class(s);
        if (g_units[s].orders == ORDERS_TRADE_ROUTE) {
            ok = 0;
            if (g_units[s].x != (unsigned char)((g_units[s].flags & 0xf) + UNIT_EUROPE_X)) {
                if (is_city(g_units[s].x, g_units[s].y) < 0) {
                    sx = g_units[s].x;
                    sy = g_units[s].y;
                    relocate_unit(s, -4, -4);
                    u = top_unit_in_square(sx, sy);
                    r = find_orphan_cargo_units(u);
                    relocate_unit(s, sx, sy);
                    if (r >= 0)
                        ok = 1;
                }
            }
        }
    }
    if (ok != 0)
        room = g_unit_types[g_units[s].type].holds - g_units[s].cargo_count;
    u = top_unit(s);
    if (u == s)
        u = next_unit(u);
    relocate_unit(s, -2, -2);
    s = u;
    while (s >= 0) {
        next = next_unit(s);
        take = 0;
        if (g_units[s].orders == ORDERS_SENTRY)
            take = 1;
        idle = !take;
        if (take == 0 && (int)(signed char)g_units[s].x < 0) {
            idle = 0;
            if (g_units[s].x == (unsigned char)((g_units[s].flags & 0xf) + UNIT_EUROPE_X)) {
                if (g_units[s].orders == ORDERS_SENTRY)
                    take = 1;
            } else
                take = 1;
        }
        if (idle != 0 && terrain_is_ocean(g_units[s].x, g_units[s].y) != 0)
            take = 1;
        size = g_unit_types[g_units[s].type].size;
        if (size <= room && take != 0) {
            room -= size;
            g_units[s].orders = ORDERS_SENTRY;
            relocate_unit(s, -2, -2);
        }
        s = next;
    }
    g_boarding_in_progress = saved;
}
