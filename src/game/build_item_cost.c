/* build_item_cost -- shipped at 1018:83b7.
 *
 * Ported by tools/port.py from matched/game/build_item_cost.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *
 * The file's own Unit, UnitType, BuildingRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 build_item_kind(col_i16 item, void *idx);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */

/* 12 bytes at SEG20:0xd422 */




col_i16 build_item_cost(col_i16 item, col_i16 *tools)
{
    register col_i16 hammers;
    register col_i16 t = 0;
    col_i16 idx;
    col_i16 kind;

    kind = build_item_kind(item, &idx);
    switch (kind) {
    case 1:
        hammers = g_buildings[idx].hammers;
        t = g_buildings[idx].tools * 10;
        break;
    case 2:
        hammers = g_unit_types[idx].hammers << 5;
        if (hammers < 0x27)
            hammers = 0x27;
        else if (hammers < 0x34)
            hammers = 0x34;
        t = g_unit_types[idx].tools * 10;
        break;
    case 0:
    default:
        hammers = 0;
        break;
    }
    if (tools != 0)
        *tools = t;
    return hammers;
}
