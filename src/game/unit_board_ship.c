/* unit_board_ship -- shipped at 1038:31b3.
 *
 * Ported by tools/port.py from matched/game/unit_board_ship.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_at(col_i16 x, col_i16 y);
col_i16 unit_chain_next(col_i16 u);
col_i16 unit_chain_tail(col_i16 u);
void stack_reorder(col_i16 u);
void unit_chain_relink(col_i16 u, col_i16 a, col_i16 b);

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


extern void ship_take_aboard(col_i16 u);







col_i16 unit_board_ship(col_i16 u)
{
    register col_i16 c;
    register col_i16 found;
    col_i16 x;
    col_i16 y;
    col_i16 r;
    col_i16 ship = -1;

    stack_reorder(u);
    c = unit_chain_tail(u);
    x = g_units[c].x;
    y = g_units[c].y;
    do {
        found = 0;
        for (c = unit_at(x, y); found == 0 && c >= 0; c = unit_chain_next(c)) {
            if (g_units[c].type >= UNIT_SHIP_FIRST && g_units[c].type <= UNIT_SHIP_LAST) {
                ship = c;
                ship_take_aboard(c);
                found = 1;
            }
        }
    } while (found != 0);
    r = unit_at(x, y);
    unit_chain_relink(ship, x, y);
    return r;
}
