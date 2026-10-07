/* blow_fake_unit: candidate for 1020:8ddd (65 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: blow_fake_unit
 * name-source: sibling-build
 *
 * Drops the highest-numbered unit if its type byte is 23, through `delete_unit`. Four
 * callers, in 1018 and 1020 (colony_fortress, colony_plan, combat_fight, try_connect), and `make_fake_unit_2` is its neighbour -- so this undoes the last
 * allocation when it turns out to hold a type-23 record.
 *
 * Unit type 23 is UNIT_TEMPORARY (units.h), the record `make_fake_unit` appends and this removes.
 *
 * THE BASE IS BIASED, NOT THE INDEX. The shipped builds the address as
 *
 *     MOV DX,0xa208 / ADD DX,-0x1a / ADD BX,DX / CMP BYTE [es:bx],0x17
 *
 * -- the array base moved back one element and two bytes on, into a register, and only then
 * the scaled index added. `g_units[g_game.unit_count - 1].type` does not produce that: Borland
 * folds the `- 1` and the `+ 2` into one displacement and emits `CMP BYTE [es:bx-0x1a],0x17`
 * with nothing in DX, four bytes shorter. Nor does `(g_units + n - 1)->type` or
 * `(&g_units[n])[-1].type`.
 *
 * What produces it is `(g_units - 1)[g_game.unit_count].type` -- the ONE-BASED spelling, where
 * the pointer is biased before the subscript. Same address, different program. That is rule
 * 46, and it says something about the original source: somewhere in this module the unit
 * array was being indexed from one.
 *
 * RENAMED 2026-09-26 from free_last_unit_if_type_23: drops the last unit when it is a UNIT_TEMPORARY one -- undoing
 * make_fake_unit, which always appends.
 *
 * RENAMED 2026-09-26 from free_last_temporary_unit: the Mac build's blow_fake_unit(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_fortress; caller colony_plan; caller combat_fight; caller try_connect; callee delete_unit), the runner-up gets 4, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 *
 * C++ (2026-10-01): the game was C++, and the merged module compiles this as C++, where
 * `(g_units - 1)[g_game.unit_count]` folds the -0x1a into the displacement. The shipped code adds it to
 * DX, which C++ produces from `g_units[g_game.unit_count - 1]`.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void delete_unit(int u);

void blow_fake_unit(void)
{
    if (g_units[g_game.unit_count - 1].type == UNIT_TEMPORARY)
        delete_unit(g_game.unit_count - 1);
}
