/* route_stop_flag_nibble_set -- shipped at 1028:c4d1.
 *
 * Ported by tools/port.py from matched/game/route_stop_flag_nibble_set.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Route far *g_cur_route_stop;                  DGROUP:0x4c1c
 *
 * The file's own Route dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void route_stop_flag_nibble_set(col_i16 high, col_i16 v)
{
    if (high != 0) {
        g_cur_route_stop->f2 &= 0xf;
        g_cur_route_stop->f2 |= v << 4;
    } else {
        g_cur_route_stop->f2 &= 0xf0;
        g_cur_route_stop->f2 |= v & 0xf;
    }
}
