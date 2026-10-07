/* kill_plan: candidate for 1028:0000 (66 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Clears cell (row, col) of the 4-byte-per-entry table at SEG24:0 -- 64 entries to
 * a row, since the index is (row << 8) + (col << 2): the byte at +2 to 0xff and
 * the one at +3 to zero.  The first function of segment 1028.
 *
 * RENAMED 2026-09-26 from clear_cell: the Mac build's kill_plan(short, short): empties one entry (kind -1, priority 0) of a nation's plan row. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

void kill_plan(int nation, int col)
{
    g_plans[nation][col].kind = 0xff;
    g_plans[nation][col].priority = 0;
}
