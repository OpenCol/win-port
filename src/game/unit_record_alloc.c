/* unit_record_alloc -- shipped at 1020:8d3a.
 *
 * Ported by tools/port.py from matched/game/unit_record_alloc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_unit_count;                         SEG20:0x779e
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


extern void unit_owner_set(col_i16 u, col_i16 owner);
extern void unit_place_at(col_i16 u, col_i16 a, col_i16 b);

col_i16 unit_record_alloc(col_i16 kind, col_i16 a, col_i16 b, col_i16 owner)
{
    col_i16 u = g_unit_count;

    g_unit_count++;
    unit_owner_set(u, owner);
    g_units[u].x = 0xff;
    g_units[u].y = 0xff;
    g_units[u].type = kind != 0 ? 0xd : 0;
    g_units[u].home = 0xff;
    g_units[u].cargo_count = 0;
    unit_place_at(u, a, b);
    return u;
}
