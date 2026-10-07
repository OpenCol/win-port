/* set_father: candidate for 1020:97fa (93 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Sets or clears one bit of a bitmap 0x13c bytes to the row, addressed by row and column.
 *
 * RENAMED 2026-09-26 from map_bit_set: the Mac build's set_father(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller give_me_liberty; caller show_fathers), the runner-up gets 1, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted. The old name was generic and misleading: the bitmap is 0x13c bytes to the row, the nation record's size, so the row is a nation and the bit a founding father -- set_father's (nation, father, value).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4; father_bits at +7 */
void set_father(int nation, int father, int on)
{
    int byte = father >> 3;
    int mask = 1 << (father & 7);
    unsigned char *p = &g_nations[nation].father_bits[byte];
    if (on)
        *p |= mask;
    else
        *p &= ~mask;
}
