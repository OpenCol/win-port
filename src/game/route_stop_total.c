/* route_stop_total -- shipped at 1028:0a15.
 *
 * Ported by tools/port.py from matched/game/route_stop_total.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1028_e350(col_i16 a, col_i16 b);
col_i16 nation_score_base(col_i16 a);
col_i16 status_bits(col_i16 a, col_i16 t);
col_i16 unit_score_penalty(col_i16 a, col_i16 u, col_i16 t);

/* 0x1c */

extern col_i16 map_plane2_nibble_if_land(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */





col_i16 route_stop_total(register col_i16 a, register col_i16 u)
{
    col_i16 r;
    col_i16 t;

    t = map_plane2_nibble_if_land(g_units[u].x, g_units[u].y);
    r = nation_score_base(a) + status_bits(a, t) + unit_score_penalty(a, u, t);
    r = max_int_1028_e350(0, r);
    return r;
}
