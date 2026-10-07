/* job_desc: candidate for 1018:3c20 (43 bytes).  Verdict: RELOCATABLE.
 * symbol: job_desc
 * name-source: sibling-build
 *
 * The text id of a profession's PLURAL EXPERT title -- "Expert Farmers", "Master Sugar Planters"
 * -- the int at +2 of its 8-byte record at SEG20:0xd61a, after the 0x1c-to-0x13 fold: what
 * `europe_recruit_any` hands to `say_string` and `europe_new_turn` to
 * `popup_say`. The fold is -called and defined in-file.
 *
 * NOT FIELD 0, AND NOT A BYTE-IDENTICAL TWIN. This was `spec_rec8_field0_1018_3c20`, "the twin
 * of spec_rec8_field0_1018_3bf5, byte-identical but for the fixups" -- and the fixup is the
 * whole difference: 3bf5 reads `[ES:BX-0x29e6]`, 0xd61a, the record's +0; this one reads
 * `[ES:BX-0x29e4]`, 0xd61c, its +2. A member offset inside an extern array is folded into the
 * displacement fixup, so the comparator cannot tell the two members apart (RE_STATUS §250;
 * `tools/check_member_offsets.py` reads the shipped displacement and can).
 *
 * WHICH OF THE TWO IS WHICH comes from the JOB list resource, whose first two columns are
 * "Farmer, Expert Farmers": +0 is the singular profession and +2 the expert title. This file
 * was called `spec_name`, which was true of both.
 *
 * RENAMED 2026-09-26 from spec_expert_name: the Mac build's job_desc(short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between find_close_colony and colony_rebel, where the Mac gap holds 3 functions and the Windows one 4, so the pairing inside it is by size and arity (the Mac body is 0.98x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "jobs.h"
#include "records.h"

extern int job_index(int spec);

int job_desc(int spec)
{
    return g_spec_recs[job_index(spec)].expert;
}
