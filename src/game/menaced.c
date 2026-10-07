/* menaced: candidate for 1028:0df4 (426 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 426, not the census 417. The trailing 9 bytes are the dead second epilogue.
 *
 * THE FIRST FOREIGN NATION NEXT TO (x, y) THAT COUNTS AS A THREAT, or -1. The eight neighbours
 * are walked in direction order and only those of the same kind as the square itself -- land
 * beside land, water beside water -- are looked at; the walk stops at the first answer.
 *
 * Two things on a neighbour can answer. UNITS (map plane 1 bit 0, owner in plane 2's high
 * nibble) of a nation other than `nation` go to `menace`, which for a tribe
 * also wants the unit itself; at sea the answer only stands if one of the ships in the stack
 * (types 0xd..0x12) has a non-zero attack byte -- an unarmed merchantman is no threat. A COLONY
 * or settlement (plane 1 bit 1) of another nation is asked too, and the first European one
 * found is also left in DGROUP:0x4c12 for the caller.
 *
 * The colony test runs even when the unit test has already answered, so a square with both
 * can overwrite the unit's answer with the colony's.
 *
 * The same-kind test is a `continue` -- the two-jump form out to the loop's increment -- and
 * the unit/-1 choice is an `if`/`else` with a store in each arm, not a ternary, which would
 * store once after the merge. `unit` and `owner` are two `register` locals, SI and DI in
 * declaration order (rule 53). The written-out `return` leaves a dead second epilogue (rule 44).
 *
 * RENAMED 2026-09-26 from adjacent_enemy: the Mac build's menaced(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller plan_move; caller plan_strategy; callee is_city; callee is_unit; callee next_unit), the runner-up gets 4, and the arity agrees (4).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int terrain_is_ocean(int x, int y);
extern int is_unit(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int is_city(int x, int y);

/* same object: 4-byte `push cs; call ` */
extern int menace(int nation, int kind, int flag, int unit);

int menaced(int x, int y, int nation, int flag)
{
    int nx;
    int ny;
    int sea;
    int dir;
    int r;
    int armed;
    register int unit;
    register int owner;

    r = -1;
    sea = terrain_is_ocean(x, y);
    g_adjacent_colony_nation = -1;
    for (dir = 0; r < 0 && dir < 8; dir++) {
        nx = g_dir_dx[dir] + x;
        ny = g_dir_dy[dir] + y;
        if (terrain_is_ocean(nx, ny) != sea)
            continue;
        owner = is_unit(nx, ny);
        if (owner >= 0 && owner != nation) {
            if (owner >= 4)
                unit = top_unit_in_square(nx, ny);
            else
                unit = -1;
            r = menace(nation, owner, flag, unit);
            if (r >= 0 && sea != 0) {
                armed = 0;
                for (unit = top_unit_in_square(nx, ny); unit >= 0; unit = next_unit(unit))
                    if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST
                        && g_unit_types[g_units[unit].type].attack != 0)
                        armed = 1;
                if (armed == 0)
                    r = -1;
            }
        }
        owner = is_city(nx, ny);
        if (owner >= 0 && owner != nation) {
            r = menace(nation, owner, flag, -1);
            if (r < 4 && g_adjacent_colony_nation < 0)
                g_adjacent_colony_nation = r;
        }
    }
    return r;
}
