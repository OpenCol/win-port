/* nth_guy_in_job: candidate for 1018:5a60 (79 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The nth colonist (0-based) of the current colony whose job byte
 * (Colony+0x20) is `job`, or -1.
 *
 * RENAMED 2026-09-26 from nth_colonist_with_job: the Mac build's nth_guy_in_job(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between specialty_count and make_building_table, where the Mac gap holds 2 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.24x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "records.h"

int nth_guy_in_job(unsigned char job, int nth)
{
    int r = -1;
    int k = -1;
    int colonist;

    for (colonist = 0; r < 0 && colonist < g_cur_colony_rec->pop; colonist++) {
        if (g_cur_colony_rec->jobs[colonist] == job) {
            k++;
            if (k == nth)
                r = colonist;
        }
    }
    return r;
}
