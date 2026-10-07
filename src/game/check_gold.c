/* check_gold: candidate for 1018:402b (40 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Nation n's gold: the long at +0x2a of the 316-byte record at SEG20:0xc2f4.
 *
 * RENAMED 2026-09-26 from nation_gold: the Mac build's check_gold(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller buy_this; caller orders_clear; caller orders_road; caller say_my_bucks; caller set_field), the runner-up gets 5, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */

long check_gold(int nation)
{
    return g_nations[nation].gold;
}
