/* rebel_level: candidate for 1038:4703 (168 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The population-weighted Sons of Liberty percentage over a nation's
 * colonies: each colony's colony_rebel times its population, summed
 * in longs, divided by the total population.
 *
 * RENAMED 2026-09-26 from nation_sol_average: the Mac build's rebel_level(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller rebel_check; callee colony_rebel; callee set_colony), the runner-up gets 2, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern void set_colony(int colony);
extern int colony_rebel(void);

int rebel_level(int nation)
{
    register int colony;
    long pop = 0;
    long sum = 0;

    for (colony = 0; colony < g_game.colony_count; colony++) {
        set_colony(colony);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        pop += (long)g_cur_colony_rec->pop;
        sum += (long)colony_rebel() * (long)g_cur_colony_rec->pop;
    }
    if (pop != 0)
        sum = sum / pop;
    return (int)sum;
}
