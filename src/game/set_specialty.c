/* set_specialty: candidate for 1018:4ccb (96 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Specialty s (0x17 folded to 0x15) for colonist i of the current colony: colonists below
 * the population count (+0x1f) live in the colony's byte array at +0x40; the rest are units
 * looked up through nth_unit_with_job(i - pop), byte +0x17.
 *
 * RENAMED 2026-09-26 from colonist_specialty_set: the Mac build's set_specialty(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller colony_plan; caller colony_select_profession; caller create_colony; caller give_me_liberty; caller new_turn_colony), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int nth_unit_with_job(int k);
void set_specialty(int i, int s)
{
    if (s == 0x17)
        s = 0x15;
    if (g_cur_colony_rec->pop > i)
        g_cur_colony_rec->spec[i] = s;
    else
        g_units[nth_unit_with_job(i - g_cur_colony_rec->pop)].spec = s;
}
