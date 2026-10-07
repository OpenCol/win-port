/* is_horse: candidate for 1038:30d9 (100 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when the unit is one of the four types that carry horses -- Dragoons, Scouts, Mounted Braves,
 * Mounted Warriors -- else 0.
 *
 * WHAT NAMES IT is `negotiate_tribe`, where a village's gift is dealt with by goods: for
 * GOODS_HORSES it asks this, and a unit that answers 0 is UPGRADED in place (`type++`, colonist to
 * scout, braves to mounted braves) while a unit that answers 1 has the horses put into the tribe's
 * own stock instead. So the question the shipped code is asking is "does this unit already have
 * horses?", and the set is the answer rather than the definition. `is_armed` is the same
 * shape one goods along, and `indian_move` uses the two together to decide how cheaply a
 * unit can retreat: mounted quarters the penalty, armed halves it, and a Dragoon is both.
 *
 * RENAMED 2026-09-26 from unit_has_horses: the Mac build's is_horse(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between set_unit_dest and find_orphan_cargo_units, which are already bound, paired in order; the arity agrees and the Mac body is 1.10x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

int is_horse(int u)
{
    return g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_MOUNTED_BRAVES || g_units[u].type == UNIT_MOUNTED_WARRIORS;
}
