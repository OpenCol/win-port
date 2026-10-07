/* custom_sell: candidate for 1018:3d17 (49 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Tests one bit of the bitmap at +0x8a of a global record, returning the bit's mask rather than 0 or 1.
 *
 * RENAMED 2026-09-26 from bitmap_test: the Mac build's custom_sell(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DrawMarket; caller colony_custom_house; caller new_turn_colony), the runner-up gets 2, the arity agrees (1), and the Mac body is 1.22x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour. It tests the bit block at Colony+0x8a that custom_set (colony_flag2_set) writes: the custom house's per-goods sell flags.
 */

#include "colonization.h"

#include "records.h"

int custom_sell(int n)
{
    return g_cur_colony_rec->flags2[n >> 3] & (1 << (n & 7));
}
