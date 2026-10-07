/* foreign_unit: candidate for 1028:3296 (338 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Carries out whatever standing order a unit has. The `switch` is on the orders byte at +8 --
 * the same byte `order_to` writes -- and dispatches 7, 8, 9 and 11 (12 too) to four
 * different workers, with everything else, 10 included, falling through to `end_unit_turn`.
 *
 * The block in front of it is one special case, and it only fires for a unit already ON a
 * mission (orders 11) whose type has bit 0 set in +0xd of the 14-byte unit-type record at SEG20:0x7ac8 and that
 * `zoc_4` says is next to someone else's land: mission 0x45 on such a square
 * decrements a per-nation counter at DGROUP:0x4b70. Trespassing, charged once.
 *
 * `plan_move` then gets a look at the unit and can stop the dispatch entirely.
 *
 * THE JUMP TABLE HAS SIX ENTRIES for four cases: `CMP BX,5` after subtracting 7 means Borland
 * built it over 7..12, so cases 11 and 12 exist in the source and share the default's code.
 * Four cases and a default would have given `CMP BX,3`.
 *
 * AND THE ARMS WERE PERMUTED, the same way `GameBrains_ProcessActiveUnit`'s were and for the same reason:
 * the table is past the census size and is never compared, so any assignment of the five arms
 * to the six cases grades the same. Read out of the shipped bytes at SEG6:0x33dd it is
 *
 *      case  7 -> 0x33b6   case  9 -> 0x33ab   case 11 -> 0x33c1
 *      case  8 -> 0x33a0   case 10 -> 0x33cc   case 12 -> 0x33c1
 *
 * with the arms at 0x33a0 `perform_orders_clear`, 0x33ab `perform_orders_road`, 0x33b6
 * `perform_orders_build`, 0x33c1 `perform_orders_move_to` and the default at 0x33cc. So it is
 * ORDERS_BUILD_COLONY that founds the colony, ORDERS_PLOW that clears or ploughs,
 * ORDERS_BUILD_ROAD that takes a work step, and ORDERS_MISSION that walks toward the
 * destination -- each order reaching the worker its own name predicts, which is what says the
 * table is read right. It was case 7 -> `perform_orders_clear` before.
 *
 * `!(x & 1)` AND NOT `(x & 1) == 0`. The shipped loads the byte, widens it with `MOV AH,0` and
 * then tests AL; the `== 0` spelling folds the whole thing into one `TEST BYTE [mem],1` and is
 * two bytes shorter. Rule 59.
 *
 * `f7` is a SIGNED char -- the comparison against 0x45 goes through `CBW`, where an
 * `unsigned char` would use `MOV AH,0`. `order_to` writes the same byte and
 * declares it unsigned; only this comparison can tell.
 *
 * 338 bytes and not the 339 the gap suggests: the last byte is alignment padding in front of
 * `purchase_available`.
 *
 * RENAMED 2026-09-26 from unit_run_mission: the Mac build's foreign_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller foreign_turn; callee perform_orders_build; callee plan_move), the runner-up gets 1, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern int zoc_4(int x, int y, int nation);
extern void end_unit_turn(int u);
extern int plan_move(int u);
extern void perform_orders_clear(int u);
extern void perform_orders_road(int u);
extern void perform_orders_build(int u);
extern void perform_orders_move_to(int u);

void foreign_unit(register int u)
{
    if (g_units[u].moves_used != 0 && g_units[u].orders == ORDERS_MISSION) {
        if (!(g_unit_types[g_units[u].type].bits & 1))
            goto work;
        if (zoc_4(g_units[u].x, g_units[u].y,
                                           NATION(g_units[u])) == 0)
            goto work;
        if ((int)g_units[u].mission == 0x45)
            g_nation_ships_in_transit[NATION(g_units[u])]--;
    }
    if (plan_move(u) != 0)
        goto out;
extern work: switch (g_units[u].orders);
out:
    ;
}
