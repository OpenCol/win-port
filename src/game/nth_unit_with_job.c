/* nth_unit_with_job: candidate for 1018:456c (92 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: nth_unit_with_job
 *
 * Walks the unit chain that starts at DGROUP:0x4bb8 (the current colony's
 * tile) and returns the n-th unit that is a colonist, or -1.  The colonist
 * test is -called, so it is defined in-file.
 *
 * RENAMED 2026-09-26 from nth_colonist_unit: the Mac build's nth_unit_with_job(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_select_profession; caller get_job; caller get_specialty; caller set_job; callee next_unit), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int next_unit(int u);

extern int is_colonist(int u);

int nth_unit_with_job(int n)
{
    int found = -1;
    register int i = -1;
    register int u;

    for (u = g_tile_first_unit; u >= 0 && found < 0; u = next_unit(u)) {
        if (is_colonist(u) != 0) {
            i++;
            if (i == n)
                found = u;
        }
    }
    return found;
}
