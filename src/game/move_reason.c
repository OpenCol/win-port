/* move_reason -- shipped at 1020:e021.
 *
 * Ported by tools/port.py from matched/game/move_reason.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern int g_move_reason;                            DGROUP:0x4c02
 *   extern int g_move_reason_unit;                       DGROUP:0x4c04
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "nations.h"
#include "terrain.h"
#include "units.h"
#define NATION(u) ((unsigned char)((u).flags & 0xf))
#define IS_SHIP(t) ((t) >= UNIT_SHIP_FIRST && (t) <= UNIT_SHIP_LAST)
extern col_i16 square_nation(col_i16 x, col_i16 y);
extern col_i16 square_nation_any(col_i16 x, col_i16 y);
extern void unit_settle_or_move(col_i16 u);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);
extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 stack_cargo_room(col_i16 u, col_i16 a, col_i16 b);
extern col_i16 square_european_nation(col_i16 x, col_i16 y);
extern col_i16 unit_chain_tail(col_i16 u);
extern unsigned char unit_move_points(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);
extern unsigned char map_plane2_nibble(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);
extern void unit_chain_relink(col_i16 u, col_i16 x, col_i16 y);

col_i16 move_reason(register col_i16 u, register col_i16 x, col_i16 y)
{
    col_i16 ok;
    col_i16 p0dst;
    col_i16 p0src;
    col_i16 tdst;
    col_i16 tsrc;
    col_i16 type;
    col_i16 sx;
    col_i16 sy;
    col_i16 top;
    col_i16 target;
    col_i16 owner;
    col_i16 nat_dst;
    col_i16 any_dst;
    col_i16 saved;

    ok = 0;
    g_move_reason = 0;
    if (y < 1)
        goto out;
    if (g_map_h - 1 <= y)
        goto out;
    if ((*g_map_width) - 1 <= x || x < 1) {
        if (IS_SHIP(g_units[u].type))
            g_move_reason = 4;
        goto out;
    }
    sx = g_units[u].x;
    sy = g_units[u].y;
    nat_dst = square_nation(x, y);
    any_dst = square_nation_any(x, y);
    unit_settle_or_move(u);
    type = g_units[u].type;
    p0dst = map_plane0_at(x, y);
    p0src = map_plane0_at(sx, sy);
    tdst = (unsigned char)(p0dst & MAP0_TERRAIN_MASK);
    tsrc = (unsigned char)(p0src & MAP0_TERRAIN_MASK);
    if (tdst == TERRAIN_OCEAN || tdst == TERRAIN_SEA_LANE) {
        if (!IS_SHIP(g_units[u].type)) {
            top = unit_at(x, y);
            if (top < 0)
                goto relink;
            if ((unsigned char)(g_units[top].flags & UNIT_NATION_MASK) != (unsigned char)(g_units[u].flags & UNIT_NATION_MASK))
                goto relink;
            if (!stack_cargo_room(top, g_unit_types[g_units[u].type].size, 0))
                goto relink;
        }
    }
    if (tdst != TERRAIN_OCEAN && tdst != TERRAIN_SEA_LANE) {
        if (IS_SHIP(g_units[u].type)) {
            if (NATION(g_units[u]) == nat_dst)
                goto land_ok;
            if (nat_dst >= NATION_EUROPEAN_COUNT)
                goto land_ok;
            if (nat_dst >= 0 && nat_dst < NATION_EUROPEAN_COUNT)
                goto land_ok;
            if (any_dst >= 0 && NATION(g_units[u]) != any_dst)
                goto relink;
            if (tsrc == TERRAIN_OCEAN || tsrc == TERRAIN_SEA_LANE || square_european_nation(sx, sy) >= 0) {
                target = -1;
                saved = u;
                for (u = unit_chain_tail(u); target < 0 && u >= 0; u = unit_chain_next(u)) {
                    if (g_unit_types[g_units[u].type].size < 99 && unit_move_points(u) > g_units[u].moves)
                        target = u;
                }
                u = saved;
                if (target >= 0) {
                    g_move_reason_unit = target;
                    if (p0dst & MAP0_RIVER)
                        g_move_reason = 3;
                    else
                        g_move_reason = 2;
                }
            }
            goto relink;
        land_ok:
            ;
        } else {
            if ((tsrc == TERRAIN_OCEAN || tsrc == TERRAIN_SEA_LANE) && any_dst >= 0
                && NATION(g_units[u]) != any_dst) {
                g_move_reason = 9;
                goto relink;
            }
        }
    }
    if (IS_SHIP(g_units[u].type)) {
        if (tdst == TERRAIN_OCEAN || tdst == TERRAIN_SEA_LANE) {
            if (map_plane2_nibble(x, y) != 1) {
                g_move_reason = 8;
                goto relink;
            }
            owner = square_nation_bit0(x, y);
            if (owner >= 0 && NATION(g_units[u]) != owner
                && (g_units[u].type < UNIT_PRIVATEER || g_units[u].type > UNIT_MAN_O_WAR)) {
                g_move_reason = 7;
                goto relink;
            }
        }
        if (tdst == TERRAIN_SEA_LANE && tsrc == TERRAIN_SEA_LANE && sx < x
            && g_units[u].orders != 3 && g_units[u].orders != ORDERS_TRADE_ROUTE) {
            g_move_reason = 5;
            goto relink;
        }
    }
    ok = 1;
relink:
    unit_chain_relink(u, sx, sy);
out:
    return ok;
}
