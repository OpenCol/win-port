/* preassign_colonists_to_fields: candidate for 1018:7a42 (174 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: preassign_colonists_to_fields
 *
 * Marks every colonist already in one of the colony's twenty work slots
 * (Colony+0x70) as placed, then, for each unplaced colonist whose
 * get_job value is under 9, tries preassign_colonist_to_field(c, -1); on success the
 * colonist is marked placed, otherwise set_job(c, 13) runs.  The three
 * helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-27 from colony_place_idle: Mac preassign_colonists_to_fields(void), 228 bytes, against 174. Caller: compute_colony_base_parameters. Callees: get_job, preassign_colonist_to_field (7327) and set_job, in that order, on both. Arity: void. Layout: the Mac CODE 7 run colony_indian_table, pick_up_invalid_colonists, preassign_colonist_to_field, preassign_colonists_to_fields, num_cargo_vessels(void), nth_cargo_vessel is the Windows 1018 run colony_indian_table, 72e4, 7327, 7a42, 7af0, nth_cargo_vessel, one for one between bound neighbours.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a, g_stub_b;

extern int get_job(int colonist);
extern int preassign_colonist_to_field(int colonist, int slot);
extern void set_job(int colonist, int job);

void preassign_colonists_to_fields(void)
{
    register int j;
    char i;
    char done[32];

    for (j = 0; j < g_cur_colony_rec->pop; j++)
        done[j] = 0;
    for (i = 0; (int)i < 20; i++) {
        if ((int)g_cur_colony_rec->slots[i] >= 0)
            done[g_cur_colony_rec->slots[i]] = 1;
    }
    for (j = 0; j < g_cur_colony_rec->pop; j++) {
        if (done[j] == 0) {
            if (get_job(j) < 9) {
                if (preassign_colonist_to_field(j, -1) == 0)
                    done[j] = 1;
                else
                    set_job(j, 13);
            }
        }
    }
}
