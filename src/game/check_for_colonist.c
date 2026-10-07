/* check_for_colonist: candidate for 1008:2d7a (66 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets two caller-supplied flags about a unit: the first when `job_of_unit`
 * returns a class at all, the second when the unit's type byte is 2. Neither is cleared,
 * so the caller zeroes both first -- and the single call site does exactly that.
 *
 * That call site has already tested `job_of_unit(u) >= 0` before calling, so the
 * first flag is redundant there. The function is written as the general question and the
 * caller asks only half of it.
 *
 * RENAMED 2026-09-26 from unit_class_flags: the Mac build's check_for_colonist(short, short*, short*), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller CheckMenuItems; callee job_of_unit), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted. Its two out-parameters are already named is_colonist and is_type2 -- check_for_colonist(short, short*, short*).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int job_of_unit(int u);

void check_for_colonist(register int u, int *is_colonist, int *is_type2)
{
    if (job_of_unit(u) >= 0)
        *is_colonist = 1;
    if (g_units[u].type == UNIT_PIONEERS)
        *is_type2 = 1;
}
