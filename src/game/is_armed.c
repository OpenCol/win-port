/* is_armed: candidate for 1038:313d (118 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * 1 when the unit is one of the five types that carry muskets -- Soldiers, Dragoons, Artillery,
 * Armed Braves, Mounted Warriors -- else 0.
 *
 * WHAT NAMES IT is the GOODS_MUSKETS arm of `negotiate_tribe`, which mirrors the horses arm
 * exactly: a unit answering 0 is upgraded in place (`type++`) by being given the muskets, and one
 * answering 1 leaves them in the tribe's stock. See `is_horse`.
 *
 * NOT EVERY ARMED UNIT IS IN THE SET. Regulars, Continental Army, Cavalry and Continental Cavalry
 * all carry muskets in the game and none of them is tested here. That is not an omission this
 * reconstruction can explain away -- it is what the shipped bytes test. The consistent reading is
 * that the set is the types a village gift can still upgrade or must not re-arm, not a general
 * "is armed" predicate, and the old name `unit_has_muskets` said so because that is the question the caller
 * asks, not because the set is closed under it.
 *
 * RENAMED 2026-09-26 from unit_has_muskets: the Mac build's is_armed(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between set_unit_dest and find_orphan_cargo_units, which are already bound, paired in order; the arity agrees and the Mac body is 1.08x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

int is_armed(int u)
{
    return g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS || g_units[u].type == UNIT_ARTILLERY || g_units[u].type == UNIT_ARMED_BRAVES || g_units[u].type == UNIT_MOUNTED_WARRIORS;
}
