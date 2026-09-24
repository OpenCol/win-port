/* refresh_arrived_unit_1008_006f -- shipped at 1008:006f.
 *
 * Ported by tools/port.py from matched/game/refresh_arrived_unit_1008_006f.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void unit_refresh_move_points(col_i16 u);

void refresh_arrived_unit_1008_006f(void)
{
    unit_refresh_move_points(g_selected_unit);
}
