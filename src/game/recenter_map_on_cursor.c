/* recenter_map_on_cursor -- shipped at 1038:91c3.
 *
 * Ported by tools/port.py from matched/game/recenter_map_on_cursor.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern int far g_cursor_x;                           DGROUP:0x4b30
 *   extern int far g_cursor_y;                           DGROUP:0x4b32
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MapWin_Recenter(void *o, col_i16 mx, col_i16 my);

void recenter_map_on_cursor(void)
{
    MapWin_Recenter(&g_map_window, g_cursor_x, g_cursor_y);
}
