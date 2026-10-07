/* explore_map_square: candidate for 1020:87e2 (198 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Marks (x, y) as seen by `nation`: bit (4 + nation) of the plane-3 byte; an unowned
 * square that lost_city_at does not veto gets the nation as owner in plane 2; the unit
 * there (top_unit_in_square) is told through spot_stack when `flag` is clear or the unit is
 * European; the colony there (find_colony) through set_see.
 *
 * RENAMED 2026-09-26 from map_plane3_set_nation_bit: the Mac build's explore_map_square(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller explore_around_square; callee owner_of; callee top_unit_in_square), the runner-up gets 1, and the arity agrees (4).
 */

#include "colonization.h"

#include "units.h"
#include "terrain.h"

#include "records.h"

extern unsigned char *site_loc(int x, int y);
extern char owner_of(int x, int y);
extern int lost_city_at(int x, int y);
extern void owner_set(int x, int y, int n);
extern int top_unit_in_square(int x, int y);
extern void spot_stack(int u, int nation);

extern void set_see(int colony, int nation);

void explore_map_square(int x, int y, int nation, int flag)
{
    unsigned char *p;
    unsigned char bit;
    int u;
    int colony;

    bit = (unsigned char)MAP3_NATION_BIT(nation);
    p = site_loc(x, y);
    *p |= bit;
    if ((int)owner_of(x, y) < 0 && lost_city_at(x, y) == 0)
        owner_set(x, y, nation);
    u = top_unit_in_square(x, y);
    if (flag == 0 || (unsigned char)(g_units[u].flags & UNIT_NATION_MASK) < 4)
        spot_stack(u, nation);
    colony = find_colony(x, y);
    if (colony >= 0)
        set_see(colony, nation);
}
