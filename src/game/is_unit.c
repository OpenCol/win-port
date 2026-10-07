/* is_unit: candidate for 1038:bdb7 (98 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The plane-2 high nibble at (x, y) when plane-1 bit 0 is set there; -1 otherwise or out of
 * range.
 *
 * RENAMED 2026-09-26 from square_nation_bit0: the nation of the units on (x, y): the plane-2 high nibble, read
 * only when plane-1 has MAP1_UNIT there (set by put_down_unit).
 *
 * RENAMED 2026-09-26 from square_unit_nation: the Mac build's is_unit(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller allied_invasion; caller colony_indian_table; caller find_beach; caller indian_move; caller plan_move), the runner-up gets 4, and the arity agrees (2).
 */

#include "colonization.h"

#include "terrain.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char feature_get(int x, int y);
extern char owner_of(int x, int y);
int is_unit(int x, int y)
{
    int n = -1;

    if (!map_index_in_range(x, y))
        return -1;
    if (feature_get(x, y) & MAP1_UNIT)
        n = owner_of(x, y);
    return n;
}
