/* zoc: candidate for 1038:2a2a (76 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: zoc
 *
 * On a square with no settlement (is_city < 0: no colony or village there, or off the map) returns zoc_2(x, y, n);
 * 0 otherwise.  zoc_2 is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from tile_claim: the Mac build's zoc(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller indian_move; caller move_to; caller plan_move; callee is_city), the runner-up gets 3, and the arity agrees (3). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int g_stub_a;
extern int is_city(int x, int y);

extern int zoc_2(int x, int y, int nation);

int zoc(int x, int y, int n)
{
    int r = 0;

    if (is_city(x, y) < 0)
        r = zoc_2(x, y, n);
    return r;
}
