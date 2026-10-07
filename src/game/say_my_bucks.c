/* say_my_bucks: candidate for 1018:40e6 (40 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Writes a nation's treasury into a message buffer as currency. Its two callers are the
 * window-title builders: ColonyWin_DrawTitle passes the colony's owner byte and
 * EuropeWin_DrawTitle the current nation, so this is the gold clause of a title line.
 *
 * RENAMED 2026-09-26 from say_nation_gold: the Mac build's say_my_bucks(short, char*), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller DrawTitle; caller DrawTitle; callee say_bucks), the runner-up gets 2, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */
extern long check_gold(int nation);

void say_my_bucks(int nation, char *buf)
{
    say_bucks(buf, check_gold(nation));
}
