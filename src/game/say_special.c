/* say_special: candidate for 1058:9371 (203 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends the name of a unit's job to a text buffer, or nothing. The job is +0x17 of the
 * unit record -- the profession field `unit_from_docks` stamps a code into and
 * `create_unit` defaults to 0x1c -- and it indexes an 8-byte table at
 * SEG20:0xd61a whose first word is the string id. `say_string` draws it; a negative id
 * draws nothing, which is how the function declines.
 *
 * Four rules, in the order the bytes apply them:
 *   - job 0x1c (the ordinary colonist) is displayed as job 0x13;
 *   - a unit with a non-zero TYPE, called with `flag` clear, whose job is not an ordinary
 *     one (`any_spec`), draws nothing at all -- id forced to -1;
 *   - type 1 or 4 with job 0x15 uses the id at SEG20:0xdd4a instead;
 *   - type 5 with job 0x16, and type 3 with job 0x18, both use the id at SEG20:0xdcd0.
 * The last three are separate `if`s over one value, not a chain: each is tested even after
 * an earlier one has fired.
 *
 * The job is read with `CBW`, so +0x17 is a SIGNED char here, while the type at +2 is
 * zero-extended. One record, two fields, two signednesses, and the sign matters: a negative
 * job would index backwards out of the table, which is presumably what -1 in that field is
 * for.
 *
 * THE TABLE IS 8 BYTES TO THE ROW, and `re/structures/rec28_seg20_d61a.json` says 28.
 * The shipped code scales the index with `SHL BX,3` and so do 43 other sites at this base,
 * against one that uses `IMUL BX,BX,28`. `tools/re_agent/infer_arrays.py` cannot see a
 * `SHL`, so the eighteen-to-one majority was never counted -- finding 102, and
 * `tools/array_strides.py` is the check. This file declares 8 because that is what the
 * instruction says.
 *
 * NAME: descriptive, in the house `say_*` family (`say_string`, `say_number`, `say_bucks`,
 * `say_where`). The previous name, `record_pick_image`, has it picking an image; it picks a
 * string id. Both callers are `StatWin_Stats`.
 *
 * RENAMED 2026-09-26 from say_unit_job: the Mac build's say_special(char*, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller Stats; callee say_string), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "units.h"
#include "jobs.h"
#include "misc.h"
#include "records.h"

extern int any_spec(int job);

void say_special(char *buf, int u, int flag)
{
    int type;
    register int job;
    register int id;

    job = g_units[u].spec;
    type = g_units[u].type;
    if (job == JOB_COLONIST_ALT)
        job = 0x13;
    id = g_spec_recs[job].name;
    if (type != UNIT_COLONISTS && flag == 0 && any_spec(job) == 0)
        id = -1;
    if ((type == UNIT_SOLDIERS || type == UNIT_DRAGOONS) && job == JOB_SOLDIER)
        id = g_misc_words[MISC_VETERAN];
    if (type == UNIT_SCOUTS && job == JOB_SCOUT)
        id = g_misc_words[MISC_EXPERT];
    if (type == UNIT_MISSIONARIES && job == JOB_MISSIONARY)
        id = g_misc_words[MISC_EXPERT];
    if (id >= 0)
        say_string(buf, id);
}
