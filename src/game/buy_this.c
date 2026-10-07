/* buy_this: candidate for 1028:a19d (179 bytes, ? callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * For a European nation (index below 4) whose CONTROL byte (+1 of the 52-byte record at SEG20:0x7840) is CONTROL_HUMAN nothing happens.
 * Otherwise the price of the action is computed, and if the nation's funds minus the
 * price still cover half the price, the price is charged, the current tribe record's byte +5 is bumped, bit 0x10 is set in map plane 1 at (x, y), and 1 is returned.
 *
 * RENAMED 2026-09-26 from pay_for_map_mark: the Mac build's buy_this(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller perform_orders_clear; caller perform_orders_road; callee feature_set; callee village_bribe), the runner-up gets 2, and the arity agrees (4).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "terrain.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern int village_bribe(int kind, int nation, int x, int y);
extern long check_gold(int nation);
extern void subtract_gold(int nation, long amount);
extern void feature_set(int x, int y, int mask, int on);

int buy_this(int kind, int nation, int x, int y)
{
    int done = 0;
    int price;

    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN)
        goto out;
    price = village_bribe(kind, nation, x, y);
    if (check_gold(nation) - price >= (long)(price >> 1)) {
        subtract_gold(nation, (long)price);
        g_cur_tribe_rec->count++;
        feature_set(x, y, MAP1_CLAIM_SETTLED, 1);
        done = 1;
    }
out:
    return done;
}
