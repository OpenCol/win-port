/* arrived_unit_mission -- shipped at 1008:3595.
 *
 * Ported by tools/port.py from matched/game/arrived_unit_mission.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *
 * The file's own Unit, Rec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 arrived_unit_mission(Rec *r, unsigned char b, unsigned char c)
{
    register col_i16 u = g_selected_unit;

    g_units[u].orders = 3;
    g_units[u].dest_x = b;
    g_units[u].dest_y = c;
    r->handled = 0;
    return 0;
}
