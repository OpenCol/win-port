/* set_owner: candidate for 1038:2c18 (57 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Low nibble of the unit's flag byte (+3) := n.
 *
 * RENAMED 2026-09-26 from unit_owner_set: the Mac build's set_owner(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller make_fake_unit; caller special_kill), the runner-up gets 1, and the arity agrees (2). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

void set_owner(int u, int owner)
{
    g_units[u].flags = (g_units[u].flags & 0xf0) | (unsigned char)(owner & 0xf);
}
