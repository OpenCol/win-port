/* terrain_fix_2: candidate for 1038:c257 (82 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The terrain byte's low five bits, folded by the map mode at DGROUP:0x1ca4:
 * mode 2 maps 8..23 onto 8..15, mode 3 maps 0..23 onto 0..7.
 *
 * RENAMED 2026-09-26 from terrain_class: the Mac build's terrain_fix_2(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between lost_city_at and map_find_continents, which are already bound; the arity agrees and the Mac body is 1.73x this one.
 */

#include "colonization.h"

char terrain_fix_2(int terrain)
{
    terrain = (unsigned char)(terrain & 0x1f);
    switch (g_map_mode) {
    case 2:
        if (terrain < 0x18 && terrain >= 8)
            terrain = (terrain & 7) | 8;
        break;
    case 3:
        if (terrain < 0x18)
            terrain = terrain & 7;
        break;
    }
    return terrain;
}
