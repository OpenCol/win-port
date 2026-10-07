/* set_building: candidate for 1018:4ea9 (91 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets or clears bit `bit` of the current colony's flag bytes (Colony+0x84):
 * byte bit >> 3, mask 1 << (bit & 7).
 *
 * RENAMED 2026-09-26 from colony_flag_set: the Mac build's set_building(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller create_colony; caller perform_orders_build; caller resolve_raid; caller set_job), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

void set_building(int bit, int on)
{
    int idx = bit >> 3;
    int mask = 1 << (bit & 7);
    unsigned char *p = &g_cur_colony_rec->flags[idx];

    if (on != 0)
        *p |= mask;
    else
        *p &= ~mask;
}
