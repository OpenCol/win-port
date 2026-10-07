/* set_course_for_new_world: candidate for 1030:2ede (251 bytes).  Verdict: RELOCATABLE.
 * symbol: set_course_for_new_world
 * name-source: sibling-build
 *
 * Sends a ship and everything aboard it to the high seas. `get_voyage_delay`
 * decides how many turns the crossing takes -- from the unit's +9 and +0xa, which are where it
 * is coming from -- and the SAME count is written into every unit in the chain, cargo included,
 * so the whole stack arrives together.
 *
 * EVERY PASSENGER THAT IS NOT A SHIP LEAVES THE LAND COUNT. Types 0xd to 0x12 are the ships;
 * anything outside that band decrements its nation's entry in the count at DGROUP:0x4b74 on the
 * way out, and the ship itself is not counted because it is in the band.
 *
 * The relink puts the unit at `nation - 0x18` in both coordinates -- the high-seas pseudo-square
 * §291's Europe dock walks -- so the stack leaves the map entirely.
 *
 * `(unsigned char)(flags & 0xf)` has to mask in AL: without the cast Borland widens first and
 * masks the word, which is two bytes longer and in the wrong order (rule 58).
 *
 * RENAMED 2026-09-26 from set_sail_for_europe: the Mac build's set_course_for_new_world(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller attempt_to_set_sail; caller foreign_country; caller perform_orders_route), the runner-up gets 1, and the arity agrees (1). The old name had the direction backwards: the Mac callers are attempt_to_set_sail (a ship leaving Europe), foreign_country and perform_orders_route, and the body sails either way -- the crossing time comes from where the unit is coming from.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern void stack_ship(int u);
extern int top_unit(int u);
extern int next_unit(int u);
extern void relocate_stacked_units(int u, int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_voyage_delay(int u, int tile_x, int b);

void set_course_for_new_world(register int unit)
{
    int turns;
    register int u;

    stack_ship(unit);
    turns = get_voyage_delay(unit, g_units[unit].dest_x, g_units[unit].dest_y);
    u = top_unit(unit);
    while (u >= 0) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            g_europe_land_units[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]--;
        g_units[u].counter = turns;
        u = next_unit(u);
    }
    relocate_stacked_units(unit, (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK) - 0x18,
                                (unsigned char)(g_units[unit].flags & UNIT_NATION_MASK) - 0x18);
}
