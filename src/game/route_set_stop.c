/* route_set_stop -- shipped at 1028:cb4f.
 *
 * Ported by tools/port.py from matched/game/route_set_stop.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern RouteRec far *g_cur_route_rec;                DGROUP:0x4c18
 *   extern Route far *g_cur_route_stop;                  DGROUP:0x4c1c
 *   extern int g_route_stop;                             DGROUP:0x4c22
 *   extern int g_route_unit;                             DGROUP:0x4c26
 *
 * The file's own RouteRec, Route dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 pick_trade_route_stop(col_i16 unit, col_i16 cur, col_i16 mark_here, col_i16 offer_extra);
void empty_1028_cb41(void);
void route_delete_stop(col_i16 idx);
void route_entry_init(col_i16 v);
void set_route(col_i16 i);

/* 0x4a bytes at SEG20:0xc804 */
/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */





void route_set_stop(void)
{
    register col_i16 c;

    if (g_cur_route_rec->stops <= g_route_stop) {
        g_route_stop = g_cur_route_rec->stops;
        set_route(g_route_stop);
        c = pick_trade_route_stop(g_route_unit, g_cur_route_rec->first_stop, 0, 1);
        if (c < 0 || c == 0x3e8)
            goto done;
        g_cur_route_rec->stops++;
        route_entry_init(c);
    } else {
        set_route(g_route_stop);
        c = pick_trade_route_stop(g_route_unit, g_cur_route_stop->stop, 0,
                                  g_cur_route_rec->stops > 1 ? 1 : 0);
        if (c < 0)
            goto done;
        if (c != 0x3e8)
            g_cur_route_stop->stop = c;
        else
            route_delete_stop(g_route_stop);
    }
done:
    empty_1028_cb41();
}
