/* set_cargo: candidate for 1018:7ca5 (119 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets the goods type of one cargo slot of a unit: the slots are packed two to a byte at Unit+0xd, low nibble for even slots, and the byte it stored comes back.
 *
 * RENAMED 2026-09-26 from unit_set_cargo_type: the Mac build's set_cargo(short, short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in unload_cargo -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (3).
 */

#include "colonization.h"

#include "records.h"

int set_cargo(int unit, int slot, int type)
{
    int mask = 0xf0;
    int off = slot >> 1;
    int cur = g_units[unit].cargo[off];
    if (slot & 1) {
        mask = mask >> 4;
        type = type << 4;
    }
    cur = (cur & mask) | type;
    g_units[unit].cargo[off] = cur;
    return cur;
}
