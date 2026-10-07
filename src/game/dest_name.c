/* dest_name: candidate for 1028:bd17 (126 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 126, not the census 118. The last 8 bytes are an unreachable epilogue.
 *
 * The display name of trade-route stop n: the nation's own adjective for the European
 * port, which the route records as colony 0x3e7, and otherwise the colony's name straight
 * out of the record array. `ask_port` writes 0x3e7 for Europe at the other
 * end of the same convention.
 *
 * The stop is `g_cur_route_rec->stops[n]` -- ten bytes each from +0x22 of the 0x4a-byte
 * route record, which is the same arithmetic `set_route` does when it parks the
 * pointer at DGROUP:0x4c1c.
 *
 * RENAMED 2026-09-26 from route_stop_name: the Mac build's dest_name(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller TradeRoute1; callee strings), the runner-up gets 1, and the arity agrees (1). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern char *strings(int n);

char *dest_name(register int n)
{
    char *name;

    if (g_cur_route_rec->stops[n].colony == 0x3e7)
        name = strings(g_nation_name_word[g_game.turn_nation]);
    else
        name = g_colonies[g_cur_route_rec->stops[n].colony].name;
    return name;
}
