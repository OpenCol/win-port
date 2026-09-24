/* unit_set_mission -- shipped at 1028:323f.
 *
 * Ported by tools/port.py from matched/game/unit_set_mission.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
void unit_set_mission(col_i16 u, unsigned char a, unsigned char b, unsigned char c)
{
    g_units[u].mission = a;
    g_units[u].orders = ORDERS_MISSION;
    g_units[u].dest_x = b;
    g_units[u].dest_y = c;
}
