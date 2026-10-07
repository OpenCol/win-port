/* clear_strategy_row: candidate for 1028:0684 (73 bytes).  Verdict: RELOCATABLE.
 *
 * Clears all sixteen cells of row a in the second 4-byte-per-entry table at
 * SEG24:0x400 -- rows of sixteen here, where `kill_plan` indexes
 * rows of sixty-four at SEG24:0.
 *
 * RENAMED 2026-09-26 from clear_row: empties all sixteen entries of a nation's strategy row (kind -1, priority 0). It has no caller anywhere in the shipped program, so no role pairs it with the Mac build's leftover clear_plan(short); named descriptively.
 */

#include "colonization.h"

#include "records.h"

void clear_strategy_row(int nation)
{
    register int i;

    for (i = 0; i < 16; i++) {
        g_strategies[nation][i].kind = 0xff;
        g_strategies[nation][i].priority = 0;
    }
}
