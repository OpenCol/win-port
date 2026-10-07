/* kill_stack: candidate for 1038:2fdd (62 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: kill_stack
 *
 * Frees every unit record in u's chain: step to the next one first, free this one, and
 * move the saved next index down when the record just freed was below it -- because
 * `delete_unit` compacts the array, so every index above the freed slot shifts by one.
 *
 * THIS WAS PARKED AS A REGISTER PUZZLE AND WAS NOT ONE. The note in
 * reconstructed/unresolved/README.md said "SI and DI swapped against ours (`mov si,[bp+6]`
 * where we emit DI)" and listed five shapes tried: `while`, `for`, an extra local,
 * `register` on either variable, and the parameter itself as the cursor. All five kept the
 * comparison written as `if (nx > delete_unit(u)) nx--;` -- with the CALL inside the
 * condition. The shipped bytes call `delete_unit(u)` and throw the result away, then
 * compare `nx > u`: `CMP DI,SI` against the cursor, not against AX.
 *
 * Putting the call in the condition forces its return value into a register and pushes the
 * cursor out of the one the shipped code keeps it in. The register allocation was the
 * SYMPTOM. Written with the call on its own line the allocation falls out by itself and
 * neither variable needs the `register` keyword.
 *
 * The idiom is elsewhere in the corpus, which is how it was recognised:
 * `process_off_board_ships` does `delete_unit(u); if (next > u) next--; if (found > u)
 * found--;` -- the same compaction fix-up, twice, on one free.
 *
 * `delete_unit` returns void here. The parked version declared it `int` to have
 * something to compare against, and that declaration was part of the same mistake.
 *
 * RENAMED 2026-09-26 from unit_chain_free_all: the Mac build's kill_stack(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller combat_fight; callee delete_unit; callee next_unit; callee top_unit), the runner-up gets 3, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int g_stub_a;

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR. */
extern int top_unit(int u);
extern int next_unit(int u);
extern void delete_unit(int u);

void kill_stack(int u)
{
    int nx;

    u = top_unit(u);
    while (u >= 0) {
        nx = next_unit(u);
        delete_unit(u);
        if (nx > u)
            nx--;
        u = nx;
    }
}
