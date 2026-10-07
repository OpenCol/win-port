/* building_class_value: candidate for 1018:5d5c (85 bytes).  Verdict: RELOCATABLE.
 * symbol: building_class_value
 *
 * For a building the colony has (has_building on its top level),
 * the SEG21:0x1eb table entry for its class byte (BuildingRec+6); -1 otherwise.
 * Both helpers are -called and defined in-file as stubs.
 *
 * THE FIELD OFFSETS IN THIS TYPEDEF WERE TWO BYTES OUT and nothing could see it: the
 * displacement of `g_buildings[i].field` is one fixup, so the comparator masks the member
 * offset along with the array's address. `setup_building` writes +2..+6 through
 * a `char f0..f6` typedef and pins them; this one now agrees with it (§119).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int base_building(int building);
extern int has_building(int bit);

int building_class_value(int building)
{
    int r = -1;

    building = base_building(building);
    if (has_building(building) != 0)
        r = g_site_slot[g_buildings[building].family];
    return r;
}
