/* clear_pickup: candidate for 1028:05a3 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Clears the first field of all sixteen six-byte records at DGROUP:0x676 back to -1.
 *
 * RENAMED 2026-09-26 from rec6_0676_reset: empties g_cargo_pickups (SEG24:0x676), the AI's list of colonies with cargo worth
 * collecting: plan_strategy adds each colony whose surplus goods are worth shipping, with a
 * value (surplus x goods value, + 8 per turn since a ship called) and a count of 100-unit loads,
 * kept sorted by value; plan_move sends a free ship to the best value-over-distance entry and
 * takes its holds off the count.
 *
 * RENAMED 2026-09-26 from pickup_list_clear: the Mac build's clear_pickup(void): empties all sixteen g_cargo_pickups entries. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"
extern Pickup g_cargo_pickups[];   /* SEG24:0x0676 */
void clear_pickup(void) { int i; for (i = 0; i < 16; i++) g_cargo_pickups[i].colony = -1; }
