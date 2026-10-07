/* delete_colonist: candidate for 1018:4aad (219 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: delete_colonist
 *
 * Takes colonist c out of the current colony: the job and speciality bytes
 * (Colony+0x20, +0x40) and the per-colonist record (get_progress/4a39)
 * shift down, the twenty work slots drop him and renumber those above him,
 * the population and the liberty-bell total (-100) fall, and a colony that
 * is shown on screen (DGROUP:0xa12) and drops under 3 gets set_building(0,0).
 *
 * RENAMED 2026-09-26 from colony_drop_colonist: the Mac build's delete_colonist(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in set_job -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "building.h"
#include "records.h"

extern int g_stub_a;
extern void set_building(int bit, int on);

extern int get_progress(int colonist);
extern void set_progress(int colonist, int progress);

void delete_colonist(int colonist)
{
    register int j;
    register int slot;

    for (j = colonist; j < g_cur_colony_rec->pop - 1; j++) {
        g_cur_colony_rec->jobs[j] = g_cur_colony_rec->jobs[j + 1];
        g_cur_colony_rec->spec[j] = g_cur_colony_rec->spec[j + 1];
        set_progress(j, get_progress(j + 1));
    }
    for (slot = 0; slot < 20; slot++) {
        if (g_cur_colony_rec->slots[slot] == (signed char)colonist)
            g_cur_colony_rec->slots[slot] = -1;
        else if (g_cur_colony_rec->slots[slot] > (signed char)colonist)
            g_cur_colony_rec->slots[slot]--;
    }
    g_cur_colony_rec->pop--;
    g_cur_colony_rec->bells_cap -= 100;
    if (g_job_popup_active != 0 && (int)g_cur_colony_rec->pop < 3)
        set_building(BUILDING_STOCKADE, 0);
}
