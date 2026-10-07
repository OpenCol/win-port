/* add_pickup: candidate for 1028:0512 (145 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Inserts (colony, value, loads, troops) into a 16-entry table of 6-byte records kept in
 * descending order of value: at the first slot whose value is smaller or whose colony is
 * negative, the rest shift down. `troops` is plan_strategy's flag for an armed land
 * unit waiting at the colony; plan_move lets a ship that is not in its aggressive mode
 * answer only the entries that have it.
 *
 * RENAMED 2026-09-26 from insert_sorted_entry: inserts a colony into g_cargo_pickups (SEG24:0x676), the AI's list of colonies with cargo worth
 * collecting: plan_strategy adds each colony whose surplus goods are worth shipping, with a
 * value (surplus x goods value, + 8 per turn since a ship called) and a count of 100-unit loads,
 * kept sorted by value; plan_move sends a free ship to the best value-over-distance entry and
 * takes its holds off the count.
 *
 * RENAMED 2026-09-26 from pickup_list_insert: the Mac build's add_pickup(short x4): (colony, value, loads, d) into g_cargo_pickups. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"
extern Pickup g_cargo_pickups[];   /* SEG24:0x0676 */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void push_pickup(int n);
void add_pickup(int colony, int value, int loads, int troops)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (g_cargo_pickups[i].value < value || g_cargo_pickups[i].colony < 0) {
            push_pickup(i);
            g_cargo_pickups[i].colony = colony;
            g_cargo_pickups[i].value = value;
            g_cargo_pickups[i].loads = loads;
            g_cargo_pickups[i].troops = troops;
            break;
        }
    }
}
