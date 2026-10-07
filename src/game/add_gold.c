/* add_gold: candidate for 1018:4053 (114 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: add_gold
 *
 * Adds `amount` to nation n's gold (the long at +0x2a of the 316-byte record
 * at SEG20:0xc2f4), clamped to 0..999,999 through the long max/min helpers.
 * The three helpers are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from nation_add_gold: the Mac build's add_gold(short, long), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller foreign_country; caller new_turn_colony; caller subtract_gold; callee check_gold), the runner-up gets 2, the arity agrees (2), and the Mac body is 0.82x this one's size.
 */

#include "colonization.h"

#include "records.h"
extern Nation g_nations[];   /* SEG20:0xc2f4 */
extern long g_stub_l;

extern long check_gold(int nation);

void add_gold(int n, long amount)
{
    long gold;

    gold = check_gold(n) + amount;
    gold = max_long_1018_b375(gold, 0L);
    gold = min_long_1018_b3a3(gold, 999999L);
    g_nations[n].gold = gold;
}
