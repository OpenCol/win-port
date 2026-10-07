/* nth_land_unit: candidate for 1048:4abc (130 bytes).  Verdict: RELOCATABLE.
 * symbol: nth_land_unit
 *
 * The nth unit on the current tile whose type falls OUTSIDE 0xd..0x12, or -1 when there is
 * no nth one. The sibling of nth_europe_vessel, differing only in the predicate.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

int nth_land_unit(int n)
{
    register int u;
    register int i = -1;
    int found = -1;

    for (u = top_unit_in_square(g_cur_nation - 0x14, g_cur_nation - 0x14); u >= 0;
         u = next_unit(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST) {
            i++;
            if (i == n)
                found = u;
        }
    }
    return found;
}
