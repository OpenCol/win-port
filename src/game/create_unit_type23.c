/* create_unit_type23 -- shipped at 1020:8ca7.
 *
 * Ported by tools/port.py from matched/game/create_unit_type23.c.
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

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void init_rec_7c0a(col_i16 a, col_i16 b);

/* 0x1c */


extern void unit_owner_set(col_i16 u, col_i16 owner);
extern void unit_place_at(col_i16 u, col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */


col_i16 create_unit_type23(col_i16 a, col_i16 x, col_i16 y, col_i16 owner, col_i16 b)
{
    register col_i16 u;

    init_rec_7c0a(a, b);
    u = g_unit_count;
    g_unit_count++;
    unit_owner_set(u, owner);
    g_units[u].type = 0x17;
    g_units[u].home = 0xff;
    g_units[u].mission = 0x2d;
    g_units[u].cargo_count = 0;
    unit_place_at(u, x, y);
    return u;
}
