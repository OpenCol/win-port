/* unit_of_job: candidate for 1018:4552 (26 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -r-
 *
 * The unit type a job turns a colonist into, from the signed byte table at DGROUP:0xbf8, one entry
 * per unit-shaped job from JOB_COLONIST (colonist, pioneer, soldier, scout, dragoon, missionary:
 * 0, 2, 1, 5, 4, 3 -- UNIT_COLONISTS .. UNIT_MISSIONARIES).
 *
 * RENAMED 2026-09-26 from job_unit_type: the Mac build's unit_of_job(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in set_job -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.5 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "jobs.h"
/* the unit type each unit-shaped job makes, for JOB_COLONIST (19) .. JOB_MISSIONARY (24); the
   shipped displacement is the table less 19 (0xbf8 - 0x13 = 0xbe5), folded into the access */

int unit_of_job(int job) { return (g_job_unit_type - JOB_COLONIST)[job]; }
