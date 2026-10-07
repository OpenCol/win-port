/* feature_set: candidate for 1038:b87a (95 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets (on != 0) or clears the mask bits of the plane-1 byte at (x, y), when the index is
 * in range.
 *
 * RENAMED 2026-09-26 from map_plane1_set_bits: the Mac build's feature_set(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller colony_depletion; caller combat_fight; caller delete_colony; caller orders_clear; caller orders_road), the runner-up gets 3, and the arity agrees (4).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char *feature_loc(int x, int y);
void feature_set(int x, int y, int mask, int on)
{
    unsigned char *p;

    if (!map_index_in_range(x, y))
        return;
    p = feature_loc(x, y);
    if (on)
        *p |= mask;
    else
        *p &= ~mask;
}
