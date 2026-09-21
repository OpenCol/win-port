/* unit_portrait -- shipped at 1040:1de3.
 *
 * Ported by tools/port.py from matched/game/unit_portrait.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 specialist_portrait(col_i16 spec);

#include "jobs.h"
#include "units.h"
/* 14 bytes */



/* same object: 4-byte `push cs; call ` */


col_i16 unit_portrait(register col_i16 u)
{
    col_i16 icon;

    icon = g_unit_types[g_units[u].type].icon;
    if (g_units[u].type == UNIT_COLONISTS)
        icon = specialist_portrait((col_i16)g_units[u].spec);
    if (g_units[u].type == UNIT_PIONEERS && (col_i16)g_units[u].spec != JOB_PIONEER)
        icon = 0x4a;
    if (g_units[u].type == UNIT_SOLDIERS && (col_i16)g_units[u].spec != JOB_SOLDIER)
        icon = 0x4b;
    if (g_units[u].type == UNIT_DRAGOONS && (col_i16)g_units[u].spec != JOB_SOLDIER)
        icon = 0x4d;
    if (g_units[u].type == UNIT_SCOUTS && (col_i16)g_units[u].spec != JOB_SCOUT)
        icon = 0x4c;
    if (g_units[u].type == UNIT_MISSIONARIES && (col_i16)g_units[u].spec != JOB_MISSIONARY)
        icon = 0x4e;
    if (g_units[u].type == UNIT_ARTILLERY && (g_units[u].f4 & 0x80))
        icon = 0x42;
    return icon;
}
