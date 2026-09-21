/* building_levels_built -- shipped at 1018:61d8.
 *
 * Ported by tools/port.py from matched/game/building_levels_built.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *   extern int g_stub_a;                                 
 *
 * The file's own BuildingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_slot_of(col_i16 b);

/* 12 bytes at SEG20:0xd422 */





col_i16 building_levels_built(col_i16 b)
{
    register col_i16 n = 0;
    register col_i16 k;

    if (b >= 0) {
        k = building_slot_of(b);
        if (k >= 0) {
            n++;
            k = g_buildings[k].next_level;
            if (k >= 0)
                n++;
        }
    }
    return n;
}
