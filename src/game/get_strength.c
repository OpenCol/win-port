/* get_strength: candidate for 1038:4fa7 (223 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: get_strength
 *
 * REGISTER ALLOCATION (rule 40). The first-declared local is auto-enregistered into DI;
 * an explicit `register` promotes a local to SI, displacing an enregistered parameter; a
 * local that is neither first nor `register` goes to the stack. This file needs the
 * count first and plain, which is the combination that was missing when it was parked.
 *
 * The current colony's defence: 1 plus its +0xb8 word rounded to hundreds,
 * plus a sixteenth of the modified strength of every non-ship unit on the
 * tile; doubled with colony flag 2, one and a half times with flag 1; at
 * least 1.  Three helpers are -called and defined in-file (max_int
 * real, the chain walkers stubs).
 *
 * RENAMED 2026-09-26 from colony_defence_str: the Mac build's get_strength(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller king_invasion; callee get_combat; callee has_building; callee next_unit; callee top_unit_in_square), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "building.h"
#include "units.h"
#include "goods.h"
#include "records.h"

extern int g_stub_a;
extern int get_combat(int u, int attacking);
extern int has_building(int bit);

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

int get_strength(void)
{
    int u;
    register int s = 1;

    s += (g_cur_colony_rec->stock[GOODS_MUSKETS] + 50) / 100;
    for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0; u = next_unit(u)) {
        if (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            s += get_combat(u, 1) >> 4;
    }
    if (has_building(BUILDING_FORTRESS) != 0)
        s = s * 2;
    else if (has_building(BUILDING_FORT) != 0)
        s = (s * 2 + s) >> 1;
    s = max_int_1038_86a6(1, s);
    return s;
}
