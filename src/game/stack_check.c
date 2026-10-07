/* stack_check: candidate for 1038:2c51 (840 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: stack_check
 *
 * Walks every unit standing on a tile and returns a tally chosen by `kind`, a jump table of
 * fifteen criteria. Called from 38 sites across the combat, colony and tribe code:
 * siege_check_2, siege_check, indian_move, king_invasion,
 * orders_disband, negotiate_tribe and unit_rules among them.
 *
 *   0   sum of the unit type's field at +9
 *   1   nothing (the jump table sends it straight to the loop tail)
 *   2   every unit
 *   3   units of type 2
 *   4   units of type 1 or 4, and units of type 6 to 9
 *   5   units of type 5
 *   6   units of type 1 or 4 or whose specialty is 0x15, and units of type 6 to 9
 *   7   nothing
 *   8   nothing
 *   9   nothing
 *   10  units OUTSIDE type 0xd..0x12 whose type's field at +6 exceeds 1
 *   11  sum of get_combat over units whose tile being ocean agrees with
 *       their type being a naval one (0xd..0x12)
 *   12  units of type 0xb
 *   13  sum of the holds of naval units
 *   14  the largest holds among naval units that are unflagged at +4, or of type 0xb
 *
 * Four of the fifteen do nothing, which is a fact about the jump table rather than a
 * reading: the table has an entry for every value 0 to 14 and those four point at the tail.
 *
 * THE ARGUMENT IS A UNIT, NOT A TILE, whatever the function's name suggests. It is
 * copied straight into `top_unit`, whose own reconstruction indexes `g_units`
 * with it and follows the link field at +0x18; every caller passes a unit index. The
 * parameter was called `tile` and the in-file stub for `top_unit` spelled its
 * own parameter `tile` as well, contradicting that function's definition. The name
 * `stack_check` is still right -- it tallies the units standing on one tile,
 * reached through the chain from any unit on it -- but the argument is one of them.

 * SIZE: the census says 810 and the function is 840. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from tile_unit_tally: the Mac build's stack_check(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller foreign_country; caller indian_move; caller plan_move; caller plan_strategy; callee next_unit), the runner-up gets 3, and the arity agrees (2).
 */

#include "colonization.h"

#include "jobs.h"
#include "units.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int top_unit(int u);
extern int next_unit(int u);
extern int terrain_is_ocean(int x, int y);
extern int get_combat(int u, int mode);

int stack_check(int unit, int kind)
{
    register int u = unit;
    register int n = 0;

    for (u = top_unit(u); u >= 0; u = next_unit(u)) {
        switch (kind) {
        case 0:
            n += g_unit_types[g_units[u].type].hammers;
            break;
        case 2:
            n++;
            break;
        case 3:
            if (g_units[u].type == UNIT_PIONEERS)
                n++;
            break;
        case 4:
            if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS)
                n++;
            if (g_units[u].type == UNIT_REGULARS || g_units[u].type == UNIT_CONTINENTAL_CAVALRY ||
                g_units[u].type == UNIT_CAVALRY || g_units[u].type == UNIT_CONTINENTAL_ARMY)
                n++;
            break;
        case 6:
            if (g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_SOLDIERS || (int)g_units[u].spec == JOB_SOLDIER)
                n++;
            if (g_units[u].type == UNIT_REGULARS || g_units[u].type == UNIT_CONTINENTAL_CAVALRY ||
                g_units[u].type == UNIT_CAVALRY || g_units[u].type == UNIT_CONTINENTAL_ARMY)
                n++;
            break;
        case 5:
            if (g_units[u].type == UNIT_SCOUTS)
                n++;
            break;
        case 10:
            if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
                if (g_unit_types[g_units[u].type].attack > 1)
                    n++;
            }
            break;
        case 11:
            if (terrain_is_ocean(g_units[u].x, g_units[u].y) ==
                (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST))
                n += get_combat(u, 1);
            break;
        case 12:
            if (g_units[u].type == UNIT_ARTILLERY)
                n++;
            break;
        case 13:
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                n += g_unit_types[g_units[u].type].holds;
            break;
        case 14:
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                if ((g_units[u].flags2 & 0x80) == 0 || g_units[u].type == UNIT_ARTILLERY)
                    n = max_int_1038_86a6(n, g_unit_types[g_units[u].type].holds);
            }
            break;
        }
    }
    return n;
}
