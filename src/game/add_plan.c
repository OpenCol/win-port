/* add_plan: candidate for 1028:0290 (321 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: add_plan
 *
 * Records a four-byte cell in row `row` of the 0x40-entry table in segment 24. If an entry
 * already holds the same first three bytes and a fourth at least as large, nothing happens.
 * Otherwise the first entry whose fourth byte is smaller, or whose third is -1, is opened up
 * by shifting the rest down and the new cell written there.
 *
 * RENAMED 2026-09-26 from cell_note_or_insert_1028_0290: the Mac build's add_plan(short x5): (nation, x, y, kind, priority) into the 64-entry plan table at SEG24:0, the one foreign_turn and plan_strategy fill. The Mac build's AI list family in its own module: add_/push_/kill_/at_plan over one per-nation table, add_/push_/kill_strategy over a second, add_/push_/clear_pickup over the cargo list. The Windows module 1028:0000..06cd holds the same three families side by side, and the pickup trio -- whose Windows names were already settled from behaviour -- pairs one-for-one by arity (4, 1, 0), which fixes what push means here: open a gap by shifting up.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void push_plan(int nation, int i);
void add_plan(int nation, int x, int y, int kind, int priority)
{
    register int i;

    for (i = 0; i < 0x40; i++) {
        if (g_plans[nation][i].x == (signed char)x && g_plans[nation][i].y == (signed char)y &&
            g_plans[nation][i].kind == (signed char)kind && g_plans[nation][i].priority >= (signed char)priority)
            goto done;
    }
    for (i = 0; i < 0x40; i++) {
        if (g_plans[nation][i].priority < (signed char)priority || (int)g_plans[nation][i].kind == -1) {
            push_plan(nation, i);
            g_plans[nation][i].x = x;
            g_plans[nation][i].y = y;
            g_plans[nation][i].kind = kind;
            g_plans[nation][i].priority = priority;
            goto done;
        }
    }
done:
    ;
}
