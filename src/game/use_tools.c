/* use_tools: candidate for 1028:a250 (212 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Spends 20 of a pioneer's tools (Unit+0x15); when fewer than 20 remain the store is
 * zeroed and the unit reverts to UNIT_COLONISTS -- UNIT_MISSIONARIES when its job is
 * JOB_MISSIONARY -- and a human nation among the first four is told, through
 * popm. The message is the binary's own TEXT identifier USEDUPTOOLS, which
 * is what names this function: `countdown` was a guess about the byte, and Unit+0x15 is
 * tools. europe_click_unit buys 100 of them with traffic_from_europe(14, 100), goods 14
 * being tools, and set_job writes keep[GOODS_TOOLS] into the same byte.
 *
 * RENAMED 2026-09-26 from unit_spend_tools: the Mac build's use_tools(short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller perform_orders_clear; caller perform_orders_road; callee popm), the runner-up gets 2, and the arity agrees (1). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "control.h"
#include "jobs.h"
#include "units.h"
#include "records.h"

extern int popm(char *msg, int n);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

void use_tools(int u)
{
    int unit = u;

    g_units[unit].hold.w.tools -= 20;
    if (g_units[unit].hold.w.tools < 20) {
        g_units[unit].hold.w.tools = 0;
        g_units[unit].type = UNIT_COLONISTS;
        if ((int)g_units[unit].spec == JOB_MISSIONARY)
            g_units[unit].type = UNIT_MISSIONARIES;
        if (NATION(g_units[unit]) < 4 && g_nation_text[NATION(g_units[unit])].control == CONTROL_HUMAN)
            popm("USEDUPTOOLS", 3);
    }
}
