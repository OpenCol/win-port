/* building_class_value -- shipped at 1018:5d5c.
 *
 * Ported by tools/port.py from matched/game/building_class_value.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *   extern signed char far g_tbl21_1eb[];                SEG21:0x01eb
 *   extern int g_stub_a;                                 
 *
 * The file's own BuildingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 building_family_base(col_i16 b);
col_i16 current_colony_flag_get(col_i16 bit);

/* 12 bytes at SEG20:0xd422 */







col_i16 building_class_value(col_i16 b)
{
    col_i16 r = -1;

    b = building_family_base(b);
    if (current_colony_flag_get(b) != 0)
        r = g_tbl21_1eb[g_buildings[b].family];
    return r;
}
