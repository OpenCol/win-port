/* owner_set: candidate for 1038:bb54 (134 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * High nibble of the plane-2 byte at (x, y) := n, for an on-map square in range. The
 * check that a European owner (n < 4) is not overwriting a native one is an empty `if`.
 *
 * RENAMED 2026-09-26 from map_plane2_set_high_nibble: the Mac build's owner_set(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_indian_table; caller combat_fight; caller conquest; caller set_field), the runner-up gets 2, and the arity agrees (3).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int is_village(int x, int y);
extern unsigned char *continent_loc(int x, int y);
void owner_set(int x, int y, int n)
{
    unsigned char *p;

    if (!on_map(x, y))
        return;
    if (!map_index_in_range(x, y))
        return;
    if (n < 4 && is_village(x, y) >= 0)
        ;
    p = continent_loc(x, y);
    *p = (*p & 0xf) | (unsigned char)((n & 0xf) << 4);
}
