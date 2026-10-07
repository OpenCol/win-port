/* base_building: candidate for 1018:5cf0 (69 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Walks a building DOWN its family to the base level and returns that id -- Fortress and Fort
 * both come back as Stockade -- so it is how the code asks "which family is this building in".
 * A negative index comes straight back.
 *
 * IT WAS `building_top_level`, WHICH IS THE OTHER END. BuildingRec+3 points at the level BELOW
 * and +4 at the level above; this follows +3, so it ends at the head. `init_data`
 * is the setter and writes the family out in full -- `(0,-1,-1,1,++i)`, `(1,0,-1,2,i)`,
 * `(2,1,-1,-1,i)` for Stockade, Fort, Fortress -- and every caller confirms the direction by what
 * it compares against: 0 (Stockade), 9 (Town Hall), 0xc (Schoolhouse) and 0x12, each one a family
 * head that `init_data` marks with `++i`. If this returned the top they would be 2, 11
 * and 14.
 *
 * THE FIELD OFFSETS IN THIS TYPEDEF WERE TWO BYTES OUT and nothing could see it: the
 * displacement of `g_buildings[i].field` is one fixup, so the comparator masks the member
 * offset along with the array's address. `setup_building` writes +2..+6 through
 * a `char f0..f6` typedef and pins them; this one now agrees with it (§119).
 *
 * RENAMED 2026-09-26 from building_family_base: the Mac build's base_building(short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in guys_at_building -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (1).
 */

#include "colonization.h"

#include "records.h"

int base_building(int building)
{
    if (building < 0)
        goto done;
    while ((int)g_buildings[building].prev_level >= 0)
        building = g_buildings[building].prev_level;
done:
    return building;
}
