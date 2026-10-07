/* push_pickup: candidate for 1028:019c (59 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Moves entries n..14 of a table of 6-byte records up by one (entry i + 1 = entry i),
 * from the top, opening a gap at entry n. The record copy is F_SCOPY; the counter and bound
 * live in DX and BX. The shipped code pushes the destination base+6 first, then the source
 * base (tools/check_data_refs.py): the copy shifts UP, entry 15 receives old entry 14.
 *
 * RENAMED 2026-09-26 from shift_table_down: shifts entries of g_cargo_pickups (SEG24:0x676), the AI's list of colonies with cargo worth
 * collecting: plan_strategy adds each colony whose surplus goods are worth shipping, with a
 * value (surplus x goods value, + 8 per turn since a ship called) and a count of 100-unit loads,
 * kept sorted by value; plan_move sends a free ship to the best value-over-distance entry and
 * takes its holds off the count.
 * The loop copies each slot into the one above it, top-down, so slot n is left free for the
 * caller (add_pickup) to fill and old slot 15 is overwritten by old slot 14.
 *
 * RENAMED 2026-09-26 from pickup_list_shift: the Mac build's push_pickup(short): opens a gap at entry n of g_cargo_pickups; add_pickup is its only caller. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"
extern Pickup g_cargo_pickups[];   /* SEG24:0x0676 */

void push_pickup(int n)
{
    int i;

    for (i = 14; i >= n; i--)
        g_cargo_pickups[i + 1] = g_cargo_pickups[i];
}
