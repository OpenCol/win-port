/* get_icon: candidate for 1018:4d71 (248 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_icon
 *
 * Picks the picture for colonist c of the current colony.  A working colonist
 * (job <= 0x13) shows his speciality through ColonistIcon (0x1c reads as
 * 0x13); otherwise job + 0x52 when the speciality matches the job (0x15 also
 * matches 0x17), job + 0x36 when not; for jobs 0x15 and 0x17 the unit on the
 * tile is looked up and, if its type is 7 or 9, the type's own sprite byte
 * wins.  The three helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colonist_portrait: the Mac build's get_icon(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller ColonyMouseMap; caller DrawColonists; caller DrawColonyMap; callee get_job; callee get_specialty), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "jobs.h"
#include "records.h"

extern int g_stub_a;
extern int ColonistIcon(int spec);

extern int get_job(unsigned int colonist);
extern int get_specialty(int u);
extern int nth_unit_with_job(int n);

int get_icon(int colonist)
{
    int job;
    int r;
    int spec;
    int match;

    job = get_job(colonist);
    spec = get_specialty(colonist);
    if (job <= JOB_COLONIST) {
        if (spec == JOB_COLONIST_ALT)
            spec = JOB_COLONIST;
        r = ColonistIcon(spec);
    } else {
        r = job + 0x52;
        match = spec == job;
        if (spec == JOB_SOLDIER && job == JOB_DRAGOON)
            match = 1;
        if (match == 0)
            r = job + 0x36;
        if (job == JOB_SOLDIER || job == JOB_DRAGOON) {
            colonist = nth_unit_with_job(colonist - g_cur_colony_rec->pop);
            if (g_units[colonist].type == UNIT_CONTINENTAL_ARMY || g_units[colonist].type == UNIT_CONTINENTAL_CAVALRY)
                r = g_unit_types[g_units[colonist].type].icon;
        }
    }
    return r;
}
