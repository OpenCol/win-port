/* set_current_route -- shipped at 1028:bcc2.
 *
 * Ported by tools/port.py from matched/game/set_current_route.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern RouteRec far g_routes[];                      SEG20:0xc804
 *   extern int g_cur_route;                              DGROUP:0x4c20
 *   extern RouteRec far *g_cur_route_rec;                DGROUP:0x4c18
 *
 * The file's own RouteRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 74 bytes at SEG20:0xc804 */




void set_current_route(col_i16 n)
{
    g_cur_route = n;
    g_cur_route_rec = &g_routes[g_cur_route];
}
