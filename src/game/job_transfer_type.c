/* job_transfer_type: candidate for 1018:4e69 (64 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Classifies a colonist move: 0 = a colony colonist to a field/building job
 * (< 0x13), 2 = a colony colonist to a tile job, 1 = a unit on the tile to a
 * tile job, 3 = a unit into the colony.
 *
 * RENAMED 2026-09-26 from colonist_move_kind: the Mac build's job_transfer_type(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in job_change_legal, set_job -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.5 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

int job_transfer_type(int colonist, int job)
{
    int r;

    if (g_cur_colony_rec->pop > colonist) {
        if (job < 0x13)
            r = 0;
        else
            r = 2;
    } else {
        if (job >= 0x13)
            r = 1;
        else
            r = 3;
    }
    return r;
}
