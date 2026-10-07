/* sort_colonists_by_profession: candidate for 1018:604c (396 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: sort_colonists_by_profession
 *
 * Reorders the colony's colonists by job: gathers index, speciality, nibble
 * and job per colonist into five stack arrays, sorts the indices by job
 * (sort_pairs_by_key), writes speciality, job and nibble back in the new
 * order, and renumbers the selected colonist (DGROUP:0x4bbc) and the twenty
 * work slots.  Nothing happens below two colonists.  The two nibble helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-27 from colony_sort_colonists: Mac sort_colonists_by_profession(void), 388 bytes, against 396. Callers: ColonyWin::DrawColony and compute_colony_base_parameters (8c35) on both. Callees: get_progress, sort_8x8 (= sort_pairs_by_key) and set_progress on both. Layout: Mac guys_at_building, delete_colony, sort, job_is_useful, food_box_size; Windows guys_at_building, delete_colony, 604c, job_is_useful, food_box_size. Arity: void.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int get_progress(int colonist);
extern void set_progress(int colonist, int progress);

void sort_colonists_by_profession(void)
{
    int i;
    char slot;
    unsigned char keys[0x20];
    unsigned char idx[0x20];
    unsigned char order[0x20];
    unsigned char nib[0x20];
    unsigned char spec[0x20];

    if ((int)g_cur_colony_rec->pop < 2)
        goto done;
    for (i = 0; i < g_cur_colony_rec->pop; i++) {
        idx[i] = i;
        spec[i] = g_cur_colony_rec->spec[i];
        nib[i] = get_progress(i);
        keys[i] = g_cur_colony_rec->jobs[i];
    }
    sort_pairs_by_key(g_cur_colony_rec->pop, idx, keys);
    for (i = 0; i < g_cur_colony_rec->pop; i++)
        order[idx[i]] = i;
    for (i = 0; i < g_cur_colony_rec->pop; i++) {
        g_cur_colony_rec->spec[order[i]] = spec[i];
        g_cur_colony_rec->jobs[i] = keys[i];
        set_progress(order[i], nib[i]);
    }
    g_selected_colonist = order[g_selected_colonist];
    for (i = 0; i < 20; i++) {
        slot = g_cur_colony_rec->slots[i];
        if ((int)slot >= 0)
            g_cur_colony_rec->slots[i] = order[slot];
    }
done:
    ;
}
