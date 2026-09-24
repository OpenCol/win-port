/* build_item_name -- shipped at 1018:830f.
 *
 * Ported by tools/port.py from matched/game/build_item_name.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern BuildingRec far g_buildings[];                SEG20:0xd422
 *   extern int far g_cancel_id;                          SEG20:0xdd08
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


extern char * nth_text_string(col_i16 n);



char * build_item_name(col_i16 item)
{
    col_i16 idx;
    col_i16 kind;
    char *s = 0;

    kind = build_item_kind(item, &idx);
    switch (kind) {
    case 1:
        s = nth_text_string(g_buildings[idx].name);
        break;
    case 2:
        s = nth_text_string(g_unit_types[idx].name);
        break;
    case 0:
    default:
        s = nth_text_string(g_cancel_id);
        break;
    }
    return s;
}
