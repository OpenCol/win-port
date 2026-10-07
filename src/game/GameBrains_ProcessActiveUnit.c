/* GameBrains_ProcessActiveUnit: candidate for 1008:36b6 (216 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The third member of the `arrived_unit_*` family at 1008:3595..36b6, and the one that acts:
 * the unit at SEG20:0x7794 has a standing order in its state byte (+8 of the 0x1c-byte
 * record) and this carries it out. The caller's record gets 2 when the state was one this
 * knows and 1 when it was not, so +4 is a handled/unhandled answer rather than a result.
 *
 * The dispatch is a jump table over states 2..9, which is what makes the range readable:
 * Borland subtracts the LOWEST case value and compares against the span, and the shipped
 * `SUB AX,2 / CMP BX,7` says the cases run from 2 to 9. Six of the eight do something and
 * the last two share the default, which is why they are in the table at all -- with only
 * cases 2..7 declared the span would be 5 and the comparison `CMP BX,5`.
 *
 * THE ARMS WERE PERMUTED AND NOTHING HERE COULD SEE IT. This function graded RELOCATABLE with
 * case 2 calling `perform_orders_clear`, case 5 `perform_orders_build` and so on -- and it still grades
 * RELOCATABLE with any permutation of the six, because each arm is one call and a `jmp`, the 
 * call's target is a masked FIXUP, and the jump table itself lies past the census size and is
 * never compared. The table was read out of the shipped bytes instead, at SEG2:0x377f:
 *
 *      case 2 -> 0x3745   case 4 -> 0x376f   case 6 -> 0x376f   case 8 -> 0x36f5
 *      case 3 -> 0x371d   case 5 -> 0x375b   case 7 -> 0x3731   case 9 -> 0x3709
 *
 * and the arms sit at 0x36f5, 0x3709, 0x371d, 0x3731, 0x3745, 0x375b with the default at
 * 0x376f. EVERY CASE THEN READS AS ITS ORDER, which is the confirmation: ORDERS_TRADE_ROUTE
 * checks the route, ORDERS_FORTIFY becomes fortified through `perform_orders_fortify` (which is
 * where ORDERS_FORTIFIED comes from) while ORDERS_FORTIFIED itself is already done and falls
 * to the default, ORDERS_BUILD_COLONY founds one, ORDERS_PLOW clears or ploughs, and
 * ORDERS_BUILD_ROAD takes a work step. Those six values are pinned independently, by the
 * IMMEDIATES their setters store -- `orders_build` writes 7, `orders_clear` 8,
 * `orders_road` 9 -- and an immediate is compared where a call target is not.
 *
 * Cases 3 and 4 keep their numbers: nothing sets those two.
 *
 * `perform_orders_route` is the one arm that takes a second argument, a literal 0,
 * where `orders_route` passes 1 to the same function.
 *
 * RENAMED 2026-09-27 from arrived_unit_act: Mac GameBrains::ProcessActiveUnit(void), 220 bytes, against 216. Caller: GameBrains::DoHuman on both. Callees: the six perform_orders_* (clear, road, move_to, build, route, fortify) on both, and nothing else. Layout: Mac EndUnitTurn, UnitGotoXY, MoveUnit, ProcessActiveUnit, Reset, KeyboardEvent, UnitActivate; Windows UnitGotoXY, MoveUnit, 36b6, Reset, KeyboardEvent, UnitActivate. Arity: this (GameBrains *r). mac_bind: strong on all three methods (votes 7/1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void perform_orders_clear(int u);
extern void perform_orders_road(int u);
extern void perform_orders_move_to(int u);
extern void perform_orders_build(int u);
extern void perform_orders_route(int u, int flag);
extern void perform_orders_fortify(int u);

void GameBrains_ProcessActiveUnit(GameBrains *r)
{
    r->phase = 2;
    switch (g_units[g_game.selected_unit].orders) {
    case ORDERS_PLOW:         perform_orders_clear(g_game.selected_unit); break;
    case ORDERS_BUILD_ROAD:   perform_orders_road(g_game.selected_unit); break;
    case 3:                   perform_orders_move_to(g_game.selected_unit); break;
    case ORDERS_BUILD_COLONY: perform_orders_build(g_game.selected_unit); break;
    case ORDERS_TRADE_ROUTE:  perform_orders_route(g_game.selected_unit, 0); break;
    case ORDERS_FORTIFY:      perform_orders_fortify(g_game.selected_unit); break;
    case 4:
    case ORDERS_FORTIFIED:
    default: r->phase = 1; break;
    }
}
