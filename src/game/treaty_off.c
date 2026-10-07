/* treaty_off: candidate for 1030:7f43 (116 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * clears mask in the pair flags of (a, b) and of (b, a); the check that both came back with the
 * same masked value is an empty `if` (a compiled-out assertion). Returns the (a, b) value.
 *
 * RENAMED 2026-09-26 from nation_pair_flags_clear_both: the Mac build's treaty_off(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller end_of_game_check; caller europe_new_wars; caller move_unit; callee get_treaty), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_treaty(int nation_a, int nation_b);
extern int set_treaty(int nation, int other, int flags);
int treaty_off(int nation, int other, int mask)
{
    int r1, r2;

    r1 = set_treaty(nation, other, get_treaty(nation, other) & ~mask);
    r2 = set_treaty(other, nation, get_treaty(other, nation) & ~mask);
    if ((r1 & mask) != (r2 & mask))
        ;
    return r1;
}
