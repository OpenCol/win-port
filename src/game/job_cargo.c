/* job_cargo: candidate for 1018:4b88 (137 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Lists the goods a job consumes: JOB_SOLDIER takes GOODS_MUSKETS, JOB_PIONEER GOODS_TOOLS,
 * JOB_SCOUT GOODS_HORSES, JOB_DRAGOON both muskets and horses; JOB_COLONIST and
 * JOB_MISSIONARY take nothing. Returns the count written.
 *
 * THE FIRST TWO WERE THE WRONG WAY ROUND and nothing here could see it: the arms are one
 * store and a `jmp`, so swapping them moves no compared byte, and the jump table that says
 * which is which sits past the census size. Read out of the shipped bytes the table sends
 * case 0x15 to the arm storing 0xf, so it is the SOLDIER who takes the muskets and the
 * PIONEER the tools. The reconstruction had the pioneer armed and the soldier carrying tools.

 * SIZE: the census says 125 and the function is 137. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from job_input_goods: the Mac build's job_cargo(short*, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in job_available, set_job -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (2.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

#include "jobs.h"
#include "goods.h"
int job_cargo(int *goods, int job)
{
    int n = 0;

    switch (job) {
    case JOB_SOLDIER:
        goods[n] = GOODS_MUSKETS;
        n++;
        break;
    case JOB_PIONEER:
        goods[n] = GOODS_TOOLS;
        n++;
        break;
    case JOB_SCOUT:
        goods[n] = GOODS_HORSES;
        n++;
        break;
    case JOB_DRAGOON:
        goods[n] = GOODS_MUSKETS;
        n++;
        goods[n] = GOODS_HORSES;
        n++;
        break;
    case JOB_COLONIST:
    case JOB_MISSIONARY:
        break;
    }
    return n;
}
