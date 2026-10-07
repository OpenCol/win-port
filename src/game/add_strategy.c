/* add_strategy: candidate for 1028:03d1 (321 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `add_plan`'s SHORTER twin, 0x141 bytes below it and the same program
 * with one number changed: sixteen entries to the row instead of sixty-four, in the table at
 * SEG24:0x400 rather than the one at 0. The row stride follows -- `SHL BX,6` for 0x40 bytes
 * where the other shifts by 8 -- and it shifts through `push_strategy` instead.
 *
 * Same two passes: an entry with the same first three bytes and a fourth at least as large
 * means there is nothing to do; otherwise the first entry whose fourth byte is smaller, or
 * whose third is -1, is opened up and the new cell written there. Both passes fall off the
 * end silently, so a full row of larger entries drops the cell.
 *
 * `(int)g_strategies[nation][i].kind == -1` carries rule 51's written-out cast where the three
 * comparisons above it do not: the constant does not fit in a byte the way `x`, `y` and `kind`
 * do.
 *
 * RENAMED 2026-09-26 from cell_note_or_insert_1028_03d1: the Mac build's add_strategy(short x5): the same insert into the 16-entry strategy table at SEG24:0x400, the one plan_move fills. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void push_strategy(int nation, int i);
void add_strategy(int nation, int x, int y, int kind, int priority)
{
    register int i;

    for (i = 0; i < 0x10; i++) {
        if (g_strategies[nation][i].x == (signed char)x && g_strategies[nation][i].y == (signed char)y &&
            g_strategies[nation][i].kind == (signed char)kind && g_strategies[nation][i].priority >= (signed char)priority)
            goto done;
    }
    for (i = 0; i < 0x10; i++) {
        if (g_strategies[nation][i].priority < (signed char)priority || (int)g_strategies[nation][i].kind == -1) {
            push_strategy(nation, i);
            g_strategies[nation][i].x = x;
            g_strategies[nation][i].y = y;
            g_strategies[nation][i].kind = kind;
            g_strategies[nation][i].priority = priority;
            goto done;
        }
    }
done:
    ;
}
