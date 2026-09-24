/* ships_at_capital -- shipped at 1030:330b.
 *
 * Ported by tools/port.py from matched/game/ships_at_capital.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_arrive_nearest(col_i16 u);

#include "units.h"
/* 0x1c */


extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);
extern void ship_take_aboard(col_i16 u);

/* same object: 4-byte `push cs; call ` */


void ships_at_capital(void)
{
    register col_i16 u;
    register col_i16 again;

    again = 1;
    do {
        if (again)
            u = unit_at(g_turn_nation - 0x20, g_turn_nation - 0x20);
        if (u >= 0) {
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
                ship_take_aboard(u);
                unit_arrive_nearest(u);
                again = 1;
            } else {
                u = unit_chain_next(u);
                again = 0;
            }
        }
    } while (u >= 0);
}
