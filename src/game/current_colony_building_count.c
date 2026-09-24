/* current_colony_building_count -- shipped at 1018:3e08.
 *
 * Ported by tools/port.py from matched/game/current_colony_building_count.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *
 * The file's own BuildingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 current_colony_flag_get(col_i16 bit);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 current_colony_building_count(col_i16 b)
{
    col_i16 n = 0;

    do {
        if (current_colony_flag_get(b))
            n++;
        b = g_buildings[b].next_level;
    } while (b >= 0);
    return n;
}
