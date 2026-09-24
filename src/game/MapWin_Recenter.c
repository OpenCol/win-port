/* MapWin_Recenter -- shipped at 1038:a40b.
 *
 * Ported by tools/port.py from matched/game/MapWin_Recenter.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_center_x;                       DGROUP:0x1c92
 *   extern int far g_map_center_y;                       DGROUP:0x1c94
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void window_relayout(void *w);

void MapWin_Recenter(void *w, col_i16 x, col_i16 y)
{
    g_map_center_x = x;
    g_map_center_y = y;
    window_relayout(w);
}
