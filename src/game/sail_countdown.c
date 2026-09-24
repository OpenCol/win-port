/* sail_countdown -- shipped at 1030:2fd9.
 *
 * Ported by tools/port.py from matched/game/sail_countdown.c.
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

extern col_i16 unit_at(col_i16 x, col_i16 y);
extern col_i16 unit_chain_next(col_i16 u);
extern void unit_detach_attach(col_i16 u, col_i16 x, col_i16 y);
extern void unit_unload_carrier(col_i16 u);

void sail_countdown(col_i16 from, register col_i16 to)
{
    col_i16 next;
    register col_i16 u;

    u = unit_at(g_turn_nation + from, g_turn_nation + from);
    while (u >= 0) {
        next = unit_chain_next(u);
        if (g_units[u].counter > 0)
            g_units[u].counter--;
        if (g_units[u].counter == 0) {
            unit_detach_attach(u, g_turn_nation + to, g_turn_nation + to);
            unit_unload_carrier(u);
        }
        u = next;
    }
}
