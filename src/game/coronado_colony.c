/* coronado_colony: candidate for 1020:88a8 (259 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Marks the 11x11 square around colony c as seen by `nation` (plane-3 bit 4 + nation) and,
 * for every colony found in it, flags it known to the nation (byte +0xba + nation) the first
 * time, clearing the companion byte at +0xbe + nation.
 *
 * RENAMED 2026-09-26 from reveal_around_colony: the Mac build's coronado_colony(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller create_colony; caller give_me_liberty; callee on_map), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"

#include "records.h"

extern unsigned char *site_loc(int x, int y);

void coronado_colony(int colony, int nation)
{
    int cx = g_colonies[colony].x;
    int cy = g_colonies[colony].y;
    int x;
    unsigned char bit;
    unsigned char *p;
    int y;

    for (x = cx - 5; cx + 5 >= x; x++) {
        for (y = cy - 5; cy + 5 >= y; y++) {
            if (!on_map(x, y))
                continue;
            bit = (unsigned char)MAP3_NATION_BIT(nation);
            p = site_loc(x, y);
            *p |= bit;
            colony = find_colony(x, y);
            if (colony >= 0) {
                if (g_colonies[colony].hist_pop[nation] == 0) {
                    g_colonies[colony].hist_pop[nation] = 1;
                    g_colonies[colony].hist_bld[nation] = 0;
                }
            }
        }
    }
}
