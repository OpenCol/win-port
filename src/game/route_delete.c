/* route_delete -- shipped at 1028:d088.
 *
 * Ported by tools/port.py from matched/game/route_delete.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern RouteRec far g_routes[];                      SEG20:0xc804
 *   extern RouteRec far *g_cur_route_rec;                DGROUP:0x4c18
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_trade_route_count;                  SEG20:0x77a2
 *
 * The file's own Unit, UnitType, RouteRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 no_trade_routes_defined(col_i16 kind, col_i16 route, char *msg);
col_i16 show_text_dialog_1028_e48e(char *msg);
void set_current_route(col_i16 n);
void set_name_slot_1028_e412(col_i16 slot, RouteRec *p);

#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */

/* 14 bytes at SEG20:0x7ac8 */

/* 0x4a bytes at SEG20:0xc804 */




/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 unit_route_get(col_i16 u);
extern void unit_route_set(col_i16 u, col_i16 r);
extern void unit_route_flag_set(col_i16 u, col_i16 f);





void route_delete(void)
{
    col_i16 u;
    register col_i16 i;
    col_i16 r;

    r = no_trade_routes_defined(0, 0, "TRADEDELETE");
    if (r < 0)
        goto out;
    set_current_route(r);
    set_name_slot_1028_e412(0, g_cur_route_rec);
    if (show_text_dialog_1028_e48e("SUREDELETE") != 1)
        goto out;
    for (u = 0; u < g_unit_count; u++) {
        if (g_unit_types[g_units[u].type].holds <= 0)
            continue;
        i = unit_route_get(u);
        if (i == r) {
            unit_route_set(u, 0);
            unit_route_flag_set(u, 0);
            if (g_units[u].orders == ORDERS_TRADE_ROUTE)
                g_units[u].orders = ORDERS_NONE;
        } else if (i > r) {
            i--;
            unit_route_set(u, i);
        }
    }
    for (i = r; i < g_trade_route_count - 1; i++)
        g_routes[i] = g_routes[i + 1];
    g_trade_route_count--;
out:
    ;
}
