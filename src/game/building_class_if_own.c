/* building_class_if_own: candidate for 1018:5db1 (92 bytes).  Verdict: RELOCATABLE.
 * symbol: building_class_if_own
 *
 * The SEG21:0x1eb entry for a building the colony has, but only when the
 * SEG21:0x1dc table maps that entry back to the same building; -1 otherwise.
 * has_building is -called and defined in-file as a stub.
 *
 * THE FIELD OFFSETS IN THIS TYPEDEF WERE TWO BYTES OUT and nothing could see it: the
 * displacement of `g_buildings[i].field` is one fixup, so the comparator masks the member
 * offset along with the array's address. `setup_building` writes +2..+6 through
 * a `char f0..f6` typedef and pins them; this one now agrees with it (§119).
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;

extern int has_building(int bit);

int building_class_if_own(int bit)
{
    register int r = -1;
    register int slot;

    if (has_building(bit) != 0) {
        slot = g_site_slot[g_buildings[bit].family];
        if (g_site_building[slot] == (signed char)bit)
            r = slot;
    }
    return r;
}
