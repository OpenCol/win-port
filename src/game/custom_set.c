/* custom_set: candidate for 1018:3d48 (91 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets or clears bit `bit` of the current colony's second flag block
 * (Colony+0x8a): byte bit >> 3, mask 1 << (bit & 7).
 *
 * RENAMED 2026-09-26 from colony_flag2_set: the Mac build's custom_set(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller colony_custom_house; caller create_colony), the runner-up gets 1, and the arity agrees (2). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

void custom_set(int bit, int on)
{
    int idx = bit >> 3;
    int mask = 1 << (bit & 7);
    unsigned char *p = &g_cur_colony_rec->flags2[idx];

    if (on != 0)
        *p |= mask;
    else
        *p &= ~mask;
}
