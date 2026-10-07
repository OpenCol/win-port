/* get_yield_type: candidate for 1018:6316 (75 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_yield_type
 *
 * The job of the colonist in grid cell (x, y), with his speciality through
 * the pointer; -1 for an empty cell.  The three helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colonist_at_cell: the Mac build's get_yield_type(short, short, short*), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller compute_yield; callee get_field; callee get_job; callee get_specialty), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

extern int g_stub_a;

extern char get_field(int x, int y);
extern int get_job(int colonist);
extern int get_specialty(int u);

int get_yield_type(int x, int y, int *spec_out)
{
    register int r = -1;
    register int colonist;

    colonist = get_field(x, y);
    if (colonist < 0)
        goto done;
    r = get_job(colonist);
    *spec_out = get_specialty(colonist);
done:
    return r;
}
