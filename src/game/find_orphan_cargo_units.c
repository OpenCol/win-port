/* find_orphan_cargo_units: candidate for 1038:31b3 (227 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: find_orphan_cargo_units
 *
 * Puts the units of u's chain aboard a ship on their tile: every ship in
 * the tile chain is taken through stack_ship (the chain is rescanned
 * after each), then the chain is relinked onto the last ship at the tile.
 * Returns the tile's first unit.  Five helpers are -called and defined
 * in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_board_ship: the Mac build's find_orphan_cargo_units(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller stack_ship; callee next_unit; callee relocate_stacked_units; callee sort_stack_by_class; callee top_unit), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int g_stub_a;
extern void stack_ship(int u);

extern void sort_stack_by_class(int u);
extern int top_unit(int u);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern void relocate_stacked_units(int u, int x, int y);

int find_orphan_cargo_units(int u)
{
    register int unit;
    register int found;
    int x;
    int y;
    int r;
    int ship = -1;

    sort_stack_by_class(u);
    unit = top_unit(u);
    x = g_units[unit].x;
    y = g_units[unit].y;
    do {
        found = 0;
        for (unit = top_unit_in_square(x, y); found == 0 && unit >= 0; unit = next_unit(unit)) {
            if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) {
                ship = unit;
                stack_ship(unit);
                found = 1;
            }
        }
    } while (found != 0);
    r = top_unit_in_square(x, y);
    relocate_stacked_units(ship, x, y);
    return r;
}
