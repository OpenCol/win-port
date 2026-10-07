/* piss_factor: candidate for 1038:0102 (55 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Buckets a 0..100 value at 25, 50 and 75 into 0..3.
 *
 * RENAMED 2026-09-26 from quarter_of_100: the Mac build's piss_factor(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller ReportsIndian; caller compute_size; caller indian_move; caller indian_village; caller plan_move), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

int piss_factor(int tension)
{
    int r;

    if (tension < 25)
        r = 0;
    else if (tension < 50)
        r = 1;
    else if (tension < 75)
        r = 2;
    else
        r = 3;
    return r;
}
