/* guy_number_of_unit: candidate for 1018:45c8 (103 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: guy_number_of_unit
 *
 * The colonist number a unit on the colony tile answers to: population plus
 * its position among the colonist units in the chain from DGROUP:0x4bb8;
 * -1 when the unit is not in the chain.  The colonist test is -called
 * and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from unit_colonist_index: the Mac build's guy_number_of_unit(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller DrawFort; caller colony_add_unit_2; caller colony_add_unit; caller create_colony; callee next_unit), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;
extern int next_unit(int u);

extern int is_colonist(int u);

int guy_number_of_unit(int target)
{
    int r = -1;
    register int index = -1;
    register int u;

    for (u = g_tile_first_unit; u >= 0 && r < 0; u = next_unit(u)) {
        if (is_colonist(u) != 0) {
            index++;
            if (u == target)
                r = g_cur_colony_rec->pop + index;
        }
    }
    return r;
}
