/* move_rate: candidate for 1038:21b1 (149 bytes, 16 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A unit's rating: the byte at +4 of its type's 14-byte table entry, plus three when the
 * owning nation (the low nibble of byte +3, masked as a byte) has bit 5 and the type is 13..18.
 *
 * RENAMED 2026-09-26 from unit_move_points: the Mac build's move_rate(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller Stats; caller combat_fight; caller indian_move; caller move_unit; caller plan_move), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "fathers.h"
#include "units.h"
#include "records.h"

extern int has_father(int nation, int bit);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

unsigned char move_rate(int u)
{
    int unit = u;
    unsigned char r;

    r = g_unit_types[g_units[unit].type].moves;
    if (has_father(NATION(g_units[unit]), FATHER_FERDINAND_MAGELLAN) && g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST)
        r += 3;
    return r;
}
