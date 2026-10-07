/* push_plan: candidate for 1028:00fc (80 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Opens a gap at index n in row `row` of the 4-byte table at SEG24:0 (rows of
 * sixty-four): every cell from 0x3e down to n moves up one.  The cell is
 * copied as a single 32-bit move, so it is four bytes wide to the source too --
 * and a four-byte struct assignment is that move, so the `long` view the file
 * used to declare (g_cells_l) was the same table under a second name.
 *
 * RENAMED 2026-09-26 from shift_cells_up: the Mac build's push_plan(short, short): opens a gap at entry n of a nation's plan row; add_plan is its only caller. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

void push_plan(int nation, int n)
{
    register int i;

    for (i = 0x3e; i >= n; i--)
        g_plans[nation][i + 1] = g_plans[nation][i];
}
