/* job_is_useful: candidate for 1018:61d8 (74 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: job_is_useful
 *
 * 0, 1 or 2: whether the job maps to a building at all (building_of_job; no colony is read) and whether
 * that level has a further one above it (BuildingRec+4, the next level up).
 * building_of_job is -called and defined in-file as a stub.
 *
 * THE FIELD OFFSETS IN THIS TYPEDEF WERE TWO BYTES OUT and nothing could see it: the
 * displacement of `g_buildings[i].field` is one fixup, so the comparator masks the member
 * offset along with the array's address. `setup_building` writes +2..+6 through
 * a `char f0..f6` typedef and pins them; this one now agrees with it (§119).
 *
 * RENAMED 2026-09-26 from building_levels_built: The body reads only static tables and never touches a colony. building_of_job maps the argument through the constant table at SEG21:0xf7 to a building. It returns 0 if there is none, 1 if the building has no next_level (BuildingRec+4), and 2 if it has one: the number of levels in that job's building line, capped at 2. preassign_colonist_to_field adds this to a job's weight. 'built' says the function counts what a colony has built, but no colony state is read.
 *
 * RENAMED 2026-09-27 from job_building_level_count: the Mac build's job_is_useful, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Maps a job through building_of_job and returns 0/1/2 by whether a building and its next level exist; Mac job_is_useful's only call is building_of_job; 1 arg, ratio 1.19; preassign_colonist_to_field adds the result to a job's weight.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int building_of_job(int job);

int job_is_useful(int job)
{
    register int n = 0;
    register int building;

    if (job >= 0) {
        building = building_of_job(job);
        if (building >= 0) {
            n++;
            building = g_buildings[building].next_level;
            if (building >= 0)
                n++;
        }
    }
    return n;
}
