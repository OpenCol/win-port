/* colony_rebel: candidate for 1018:3c91 (134 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_rebel
 *
 * The colony's Sons of Liberty percentage: bells (Colony+0xc2) * 100 over
 * the bells needed (+0xc6) when that is positive, +20 when the nation has
 * bit 0x12 set (has_father), capped at 100.  min_int is -called and
 * defined in-file.
 *
 * RENAMED 2026-09-26 from colony_sol_percent: the Mac build's colony_rebel(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller DrawColonists; caller combat_fight; caller new_turn_colony; callee has_father), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "fathers.h"
#include "records.h"

extern int has_father(int nation, int bit);

int colony_rebel(void)
{
    long percent = 0;
    long bells = g_cur_colony_rec->bells;
    register int pct;

    if (g_cur_colony_rec->bells_cap > 0)
        percent = bells * 100 / g_cur_colony_rec->bells_cap;
    pct = (int)percent;
    if (has_father(g_cur_colony_rec->nation, FATHER_SIMON_BOLIVAR) != 0)
        pct += 20;
    pct = min_int_1018_b329(100, pct);
    return pct;
}
