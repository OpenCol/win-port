/* master_unit_in_stack: candidate for 1038:2185 (44 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: master_unit_in_stack
 *
 * sort_stack_by_class on the unit, then the tail of its chain.  Both helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from unit_prepare_tail: the Mac build's master_unit_in_stack(short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (caller move_unit; callee sort_stack_by_class; callee top_unit), the runner-up gets 2; the arity agrees (1), and the body does what the Mac name says.
 */

#include "colonization.h"

extern int g_stub_a;

extern void sort_stack_by_class(int u);
extern int top_unit(int u);

int master_unit_in_stack(int u)
{
    sort_stack_by_class(u);
    u = top_unit(u);
    return u;
}
