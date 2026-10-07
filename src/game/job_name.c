/* job_name: candidate for 1018:3bf5 (43 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: job_name
 *
 * The text id of a profession's SINGULAR name -- "Farmer" -- the int at +0 of its 8-byte record
 * at SEG20:0xd61a, after the 0x1c-to-0x13 fold. The fold is -called and defined in-file.
 *
 * ONE RECORD, TWO TEXT IDS, TWO ACCESSORS. `job_desc` 43 bytes below is the
 * same function shape and was filed as this one's byte-identical twin, but it reads +2, the
 * PLURAL EXPERT title -- "Expert Farmers". The JOB list resource has both as its first two
 * columns and is what tells them apart; the member offset is the one thing that differs between
 * these two functions and the one thing the comparator masks (RE_STATUS §250).
 *
 * RENAMED 2026-09-26 from spec_job_name: the Mac build's job_name(short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between find_close_colony and colony_rebel, where the Mac gap holds 3 functions and the Windows one 4, so the pairing inside it is by size and arity (the Mac body is 0.98x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "jobs.h"
#include "records.h"

extern int job_index(int spec);

int job_name(int spec)
{
    return g_spec_recs[job_index(spec)].name;
}
