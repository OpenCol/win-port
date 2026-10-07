/* PORT: this file's own declarations of popi removed: the header declares it from the definition (W1 step 3). */
/* indian_make_peace: candidate for 1030:377d (192 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * ORs 0x40 into the (nation, tribe) pair flags both ways; for a human European nation
 * announces INDIANPEACE (tribe adjective, nation adjective) and, when the tribe's word
 * pissed(nation - 4, tribe) is below 25, INDIANCOME as well. Returns the pair flags.
 *
 * RENAMED 2026-09-26 from establish_tribe_peace: the Mac build's indian_make_peace(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller negotiate_tribe; callee pissed; callee popup_say), the runner-up gets 2, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 *
 * A LIKELY SHIPPED SLIP: `pissed(nation - 4, tribe)` runs only for a European `nation` (0..3),
 * so it reads g_tribes[-4..-1] -- pissed takes (tribe index, nation), and every other caller
 * passes it that way round; `pissed(tribe - 4, nation)` reads like what was meant. The bytes
 * fix the reversed order.
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "tribes.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern int treaty_on(int nation, int other, int mask);
extern int get_name(int nation);

extern void popup_say(int slot, int word);
extern int pissed(int tribe, int i);

int indian_make_peace(int nation, int tribe)
{
    int r = treaty_on(nation, tribe, 0x40);

    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN) {
        popup_say(0, get_name(tribe));
        popup_say(1, get_name(nation));
        popi("INDIANPEACE", tribe - 4);
        if (pissed(nation - 4, tribe) < TRIBE_ATTITUDE_MEDIUM) {
            popup_say(0, get_adjective(tribe));
            popi("INDIANCOME", tribe - 4);
        }
    }
    return r;
}
