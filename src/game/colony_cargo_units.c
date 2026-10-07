/* colony_cargo_units: candidate for 1018:7af0 (112 bytes).  Verdict: RELOCATABLE.
 *
 * Counts the units on the current colony's tile whose type has a hold
 * (UnitType+7 non-zero): the wagon trains and ships the colony screen
 * can load.
 */

#include "colonization.h"

#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

int colony_cargo_units(void)
{
    register int u;
    register int n = 0;

    for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = next_unit(u)) {
        if (g_unit_types[g_units[u].type].holds != 0)
            n++;
    }
    return n;
}
