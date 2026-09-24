/* best_defender -- shipped at 1020:1f8c.
 *
 * Ported by tools/port.py from matched/game/best_defender.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 14 bytes at SEG20:0x7ac8 */


extern col_i16 square_european_nation(col_i16 x, col_i16 y);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 unit_chain_tail(col_i16 u);
extern col_i16 unit_chain_next(col_i16 u);
extern col_i16 defence_bonus(col_i16 u, col_i16 attacker);
extern col_i16 unit_combat_strength(col_i16 u, col_i16 attacking);

col_i16 best_defender(col_i16 u, col_i16 attacker)
{
    col_i16 best;
    col_i16 sea;
    col_i16 euro;
    col_i16 x;
    col_i16 y;
    col_i16 have;
    col_u16 best_score;
    register col_i16 unit;
    register col_u16 score;

    best = -1;
    have = 0;
    best_score = 0;
    if (u >= 0) {
        unit = u;
        x = g_units[unit].x;
        y = g_units[unit].y;
        euro = square_european_nation(x, y) >= 0;
        if (on_map(x, y) != 0) {
            sea = ocean(x, y);
            have = 1;
        }
    }
    for (unit = unit_chain_tail(u); unit >= 0; unit = unit_chain_next(unit)) {
        score = (defence_bonus(unit, attacker) << 8)
              + (0xff - unit_combat_strength(unit, 0));
        if (g_units[unit].type == UNIT_ARTILLERY) {
            if (euro == 0) {
                if (g_units[unit].orders != ORDERS_FORTIFY && g_units[unit].orders != ORDERS_FORTIFIED)
                    score = score >> 3;
            } else if ((unsigned char)(g_units[attacker].flags & UNIT_NATION_MASK) >= 4) {
                score = score * 2;
            }
        }
        if (euro != 0 && g_unit_types[g_units[unit].type].attack == 0)
            goto next;
        if (have != 0
            && (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) != sea)
            goto next;
        if (score >= best_score) {
            best_score = score;
            best = unit;
        }
next:
        ;
    }
    return best;
}
