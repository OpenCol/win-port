/* get_combat: candidate for 1020:7e61 (438 bytes, 10 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Combat strength of a unit for one side of a fight. Base is the type table's attack
 * byte (+6) when attacking, defence byte (+5) otherwise, less 2 for type 11 with bit 0x80
 * of its byte +4; stored per side, then scaled by eight. Half again for types 1 and 4
 * whose byte +0x17 is 0x15, and for type 16 when the nation has bit 13; types 13..18
 * (ships) lose one point per unit of cargo aboard. Each bonus sets a bit in a per-side flag word.
 *
 * THE SHIP PENALTY IS THE CARGO COUNT, +0xc. This file had it as a `damage` byte at +0xa and
 * graded green, because a member offset inside an extern array is folded into the displacement
 * fixup; the shipped reads `[ES:BX-0x5dec]`, 0xa214, which 48 other files call `cargo_count`.
 * A loaded ship fights worse (RE_STATUS §250, `tools/check_member_offsets.py`).
 *
 * RENAMED 2026-09-26 from unit_combat_strength: the Mac build's get_combat(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller collect_stats; caller colony_plan; caller indian_move; caller stack_check; callee has_father), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "fathers.h"
#include "jobs.h"
#include "units.h"
#include "records.h"

extern int has_father(int nation, int bit);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

int get_combat(int unit, int attacking)
{
    int n = unit;
    int side;
    int str;
    int r;

    side = attacking ? 0 : 1;
    g_combat_flags[side] |= attacking ? 1 : 0;
    str = attacking ? g_unit_types[g_units[n].type].attack : g_unit_types[g_units[n].type].defence;
    if (g_units[n].type == UNIT_ARTILLERY && (g_units[n].flags2 & 0x80))
        str -= 2;
    g_combat_str[side] = str;
    r = str << 3;
    if ((g_units[n].type == UNIT_SOLDIERS || g_units[n].type == UNIT_DRAGOONS) && (int)g_units[n].spec == JOB_SOLDIER) {
        r += r >> 1;
        g_combat_flags[side] |= 2;
    }
    if (g_units[n].type == UNIT_PRIVATEER && has_father(NATION(g_units[n]), FATHER_FRANCIS_DRAKE)) {
        r += r >> 1;
        g_combat_flags[side] |= 0x4000;
    }
    if (g_units[n].type >= UNIT_SHIP_FIRST && g_units[n].type <= UNIT_SHIP_LAST) {
        r -= g_units[n].cargo_count;
        if (g_units[n].cargo_count != 0)
            g_combat_flags[side] |= 4;
    }
    return r;
}
