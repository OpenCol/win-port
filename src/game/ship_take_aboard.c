/* ship_take_aboard -- shipped at 1038:3296.
 *
 * Ported by tools/port.py from matched/game/ship_take_aboard.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int g_boarding_193e;                          DGROUP:0x193e
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_at(col_i16 x, col_i16 y);
col_i16 unit_board_ship(col_i16 u);
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);
void stack_reorder(col_i16 u);
void unit_detach_attach(col_i16 u, col_i16 x, col_i16 y);

#include "units.h"
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */







void ship_take_aboard(col_i16 ship)
{
    col_i16 u;
    col_i16 next;
    col_i16 room;
    col_i16 size;
    col_i16 take;
    col_i16 idle;
    col_i16 ok;
    col_i16 saved;
    col_i16 r;
    col_i16 sx;
    col_i16 sy;
    register col_i16 s;

    s = ship;
    room = 0;
    ok = 1;
    saved = g_boarding_193e;
    g_boarding_193e = 1;
    if (saved == 0) {
        stack_reorder(s);
        if (g_units[s].orders == ORDERS_TRADE_ROUTE) {
            ok = 0;
            if (g_units[s].x != (unsigned char)((g_units[s].flags & 0xf) + UNIT_EUROPE_X)) {
                if (square_nation(g_units[s].x, g_units[s].y) < 0) {
                    sx = g_units[s].x;
                    sy = g_units[s].y;
                    unit_detach_attach(s, -4, -4);
                    u = unit_at(sx, sy);
                    r = unit_board_ship(u);
                    unit_detach_attach(s, sx, sy);
                    if (r >= 0)
                        ok = 1;
                }
            }
        }
    }
    if (ok != 0)
        room = g_unit_types[g_units[s].type].holds - g_units[s].cargo_count;
    u = unit_chain_tail(s);
    if (u == s)
        u = unit_chain_next(u);
    unit_detach_attach(s, -2, -2);
    s = u;
    while (s >= 0) {
        next = unit_chain_next(s);
        take = 0;
        if (g_units[s].orders == ORDERS_SENTRY)
            take = 1;
        idle = !take;
        if (take == 0 && (col_i16)(signed char)g_units[s].x < 0) {
            idle = 0;
            if (g_units[s].x == (unsigned char)((g_units[s].flags & 0xf) + UNIT_EUROPE_X)) {
                if (g_units[s].orders == ORDERS_SENTRY)
                    take = 1;
            } else
                take = 1;
        }
        if (idle != 0 && ocean(g_units[s].x, g_units[s].y) != 0)
            take = 1;
        size = g_unit_types[g_units[s].type].size;
        if (size <= room && take != 0) {
            room -= size;
            g_units[s].orders = ORDERS_SENTRY;
            unit_detach_attach(s, -2, -2);
        }
        s = next;
    }
    g_boarding_193e = saved;
}
