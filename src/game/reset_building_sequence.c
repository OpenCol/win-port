/* reset_building_sequence: candidate for 1018:5aaf (88 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Reseeds the generator from the colony's position: random_activate with the
 * word at DGROUP:0x4b88, then random_reseed with x + (long at 0x4bc2) + (y << 8).
 *
 * RENAMED 2026-09-26 from colony_seed_random: the Mac build's reset_building_sequence(void), bound by layout (on both sides between nth_guy_in_job and make_building_table); the arity agrees and the Mac body is 1.11x this one.
 */

#include "colonization.h"

#include "records.h"

extern void random_activate(int seed);
extern void random_reseed(long seed);

void reset_building_sequence(void)
{
    long seed;

    random_activate(g_event_seed);
    seed = (long)g_cur_colony_rec->x + g_seed_base + (long)(g_cur_colony_rec->y << 8);
    random_reseed(seed);
}
