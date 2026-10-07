/* job_of_building: candidate for 1018:5d35 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The job worked in a building: the signed byte table at SEG21:0xcd, indexed by building,
 * -1 for a building nobody works in. Every caller passes a building and uses the answer
 * as a job (job_count, ColonyWin_DrawIndividual, output_of_building's job switch); the
 * header called it "the goods a job produces" until 2026-09-27.
 *
 * RENAMED 2026-09-26 from job_goods: the Mac build's job_of_building(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in guys_at_building -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

int job_of_building(int building)
{
    int r = -1;

    r = g_job_of_building[building];
    return r;
}
