/* has_building: candidate for 1018:3de8 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: has_building
 *
 * colony_has_building for the colony selected at DGROUP:0x4ba2.  The callee is
 * reached by the 4-byte `push cs / call ` form, so it is defined in-file as
 * a stub.
 *
 * RENAMED 2026-09-26 from current_colony_flag_get: the Mac build's has_building(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller DoBuildingFizzle; caller DrawABuilding; caller DrawMarket; caller colony_select_profession; caller new_turn_colony), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

extern int g_stub_a;

extern int colony_has_building(int colony, int bit);

int has_building(int bit)
{
    return colony_has_building(g_cur_colony_idx, bit);
}
