/* any_spec: candidate for 1018:3915 (53 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * 0 for the five jobs 0x13, 0x19, 0x1a, 0x1b and 0x1c, 1 for everything else.
 * One epilogue with both arms loading AX makes it a single boolean expression
 * rather than a pair of returns, and the tested-first order is the source's.
 *
 * RENAMED 2026-09-26 from is_ordinary_job: the Mac build's any_spec(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller colony_plan; caller colony_select_profession; caller fill_docks; caller foreign_country; caller new_turn_colony), the runner-up gets 5, the arity agrees (1), and the Mac body is 1.28x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83).
 */

#include "colonization.h"

#include "jobs.h"
int any_spec(int job)
{
    return job != JOB_COLONIST_ALT && job != JOB_COLONIST && job != JOB_INDENTURED_SERVANT
        && job != JOB_CRIMINAL && job != JOB_CONVERT;
}
