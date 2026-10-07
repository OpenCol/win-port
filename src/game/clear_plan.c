/* clear_plan: candidate for 1028:05cc (184 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Empties row `row` of the 64-entry table at SEG24:0 and replays the sixteen entries of the
 * same row of the second table at SEG24:0x400 back through `add_plan`.
 * An entry whose third byte is negative is skipped, which is the same "-1 means empty"
 * convention `clear_strategy_row` writes and `add_plan` looks for.
 *
 * So the small table is the authority and the big one is derived from it: this is the
 * rebuild, not a copy -- every surviving entry goes back in through the insert path, which
 * re-sorts by the fourth byte and drops duplicates.
 *
 * TWO THINGS THE CBW DECIDES, and both were wrong on the first compile.
 *
 * The guard is `(int)plan.kind >= 0`, not `plan.kind >= 0`. With the cast the shipped loads the
 * byte and sign-extends it -- `MOV AL / CBW / OR AX,AX / JL` -- and without it Borland
 * compares the byte in memory directly, `CMP BYTE [es:bx+2],0`, two bytes shorter. This is
 * the §114 flag-test rule in its other direction: what promotes and what does not is
 * decided by the expression, not the field.
 *
 * And the four arguments are pushed with `CBW` too, which means THIS translation unit sees
 * `add_plan` with `int` parameters, not `char`. Its own reconstruction
 * at 1028:0290 declares them `char` and verifies, because the callee only ever reads the low
 * byte -- §119's blind spot exactly. The declarations disagree and the bytes are happy with
 * both; a caller that promotes and a callee that truncates is the ordinary shape of a
 * 1994 header that was not in scope here.
 *
 * RENAMED 2026-09-26 from rebuild_cell_row: empties a nation's plan row (kill_plan x64) and replays its strategy row back through add_plan -- the plan table is rebuilt from the strategy table each AI turn. Named descriptively: the Mac build's two unbound one-argument names in this family, clear_plan and plan_strategy, are both candidates and nothing here separates them (plan_strategy could as well be plan_strategy, also (nation)).
 *
 * RENAMED 2026-09-26 from rebuild_plan_from_strategy: the Mac build's clear_plan(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (callee add_plan; callee kill_plan), the runner-up gets 1, and the arity agrees (1). The Mac clear_plan calls exactly kill_plan and add_plan, as this does; the Mac plan_strategy is ai_survey_and_assign.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` */
extern void kill_plan(int nation, int col);
extern void add_plan(int nation, int x, int y, int kind, int priority);

void clear_plan(int nation)
{
    register int i;

    for (i = 0; i < 0x40; i++)
        kill_plan(nation, i);
    for (i = 0; i < 0x10; i++) {
        if ((int)g_strategies[nation][i].kind >= 0)
            add_plan(nation, g_strategies[nation][i].x, g_strategies[nation][i].y,
                                          g_strategies[nation][i].kind, g_strategies[nation][i].priority);
    }
}
