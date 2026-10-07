/* collect_stats_1: candidate for 1030:2731 (241 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Counts the nation's colonists by specialty into the 29 bytes at SEG20:0xda2a: every unit
 * owned by the nation that job_of_unit accepts, plus every colonist of every colony the
 * nation owns.
 *
 * RENAMED 2026-09-26 from tally_codes_for_player: the Mac build's collect_stats_1(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller new_turn_bookkeeping; callee get_specialty; callee job_of_unit; callee set_colony), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int job_of_unit(int u);
extern void set_colony(int colony);
extern int get_specialty(int u);

void collect_stats_1(int nation)
{
    int i;
    int col;
    int spec;
    int u;
    int colonist;

    for (i = 0; i < 0x1d; i++)
        g_specialty_tally[i] = 0;
    for (u = 0; g_game.unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != (unsigned char)nation)
            continue;
        if (job_of_unit(u) >= 0)
            g_specialty_tally[g_units[u].spec]++;
    }
    for (col = 0; g_game.colony_count > col; col++) {
        set_colony(col);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        for (colonist = 0; g_cur_colony_rec->pop > colonist; colonist++) {
            spec = get_specialty(colonist);
            g_specialty_tally[spec]++;
        }
    }
}
