/* set_route -- shipped at 1028:bce8.
 *
 * Ported by tools/port.py from matched/game/set_route.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far *g_cur_route_rec;                    DGROUP:0x4c18
 *   extern char far *g_cur_route_stop;                   DGROUP:0x4c1c
 *   extern int g_route_stop;                             DGROUP:0x4c22
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_route(col_i16 i)
{
    g_route_stop = i;
    g_cur_route_stop = g_cur_route_rec + g_route_stop * 10 + 0x22;
}
