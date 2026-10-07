/* push_strategy: candidate for 1028:014c (80 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The same gap-opening shift for the second table at SEG24:0x400, whose rows
 * hold sixteen cells: every cell from 0xe down to n moves up one.
 *
 * RENAMED 2026-09-26 from shift_cells2_up: the Mac build's push_strategy(short, short): the same gap in the strategy row; add_strategy is its only caller. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

void push_strategy(int nation, int n)
{
    register int i;

    for (i = 0xe; i >= n; i--)
        g_strategies[nation][i + 1] = g_strategies[nation][i];
}
