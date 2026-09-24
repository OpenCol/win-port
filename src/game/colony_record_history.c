/* colony_record_history -- shipped at 1018:aef8.
 *
 * Ported by tools/port.py from matched/game/colony_record_history.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colony_building_count(col_i16 c, col_i16 b);

/* 0xca bytes */





void colony_record_history(col_i16 c, col_i16 k)
{
    ((unsigned char *)((unsigned char *)&(g_colonies[c]) + 0xba))[k] = g_colonies[c].pop;
    ((unsigned char *)((unsigned char *)&(g_colonies[c]) + 0xbe))[k] = colony_building_count(c, 0);
}
