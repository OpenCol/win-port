/* colony_building_top -- shipped at 1018:3ece.
 *
 * Ported by tools/port.py from matched/game/colony_building_top.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern BuildingLink far g_building_links[];          SEG20:0xd426
 *
 * The file's own BuildingLink dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 current_colony_flag_get(col_i16 bit);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 colony_building_top(col_i16 b)
{
    col_i16 top = -1;

    while (b >= 0) {
        if (!current_colony_flag_get(b))
            break;
        top = b;
        b = g_building_links[b].link;
    }
    return top;
}
