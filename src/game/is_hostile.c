/* is_hostile: candidate for 1038:bfdd (137 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The European nation owning (x, y) when it is not `me` and me's pair-flag byte for it has
 * bit 0x40 set (plane-1 bits 0x48 mark an owned square); -1 otherwise or out of range.
 *
 * RENAMED 2026-09-26 from foreign_nation_at: the Mac build's is_hostile(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller close_move; caller try_connect; callee feature_get; callee owner_of), the runner-up gets 1, and the arity agrees (3).
 */

#include "colonization.h"

#include "w16types.h"
#include "terrain.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */
/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char feature_get(int x, int y);
extern char owner_of(int x, int y);
int is_hostile(int x, int y, int me)
{
    int r = -1;
    int n;

    if (!map_index_in_range(x, y))
        return -1;
    if (feature_get(x, y) & (MAP1_ROAD | MAP1_PLOWED)) {
        n = owner_of(x, y);
        if (n >= 0 && n < 4 && n != me && (g_nations[me].rel[n] & 0x40))
            r = n;
    }
    return r;
}
