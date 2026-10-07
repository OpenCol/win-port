/* colony_max_size: candidate for 1018:3f45 (64 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_max_size
 *
 * How many ring tiles the colony's land covers: 4, 8 or 12 for ring levels
 * 1..3, 32 otherwise.  colony_map_status is -called, defined in-file
 * as a stub.
 *
 * RENAMED 2026-09-26 from colony_ring_tiles: the Mac build's colony_max_size(void), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in colony_plan -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.5 votes, the next candidate 0.5); the arity agrees (0).
 */

#include "colonization.h"

extern int g_stub_a;

extern int colony_map_status(void);

int colony_max_size(void)
{
    register int r;

    switch (colony_map_status()) {
    case 1:
        r = 4;
        break;
    case 2:
        r = 8;
        break;
    case 3:
        r = 12;
        break;
    default:
        r = 0x20;
        break;
    }
    return r;
}
