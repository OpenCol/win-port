/* mark_corner -- shipped at 1020:0000.
 *
 * Ported by tools/port.py from matched/game/mark_corner.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short far g_plane3_handle;                    DGROUP:0x1c70
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void map_handle_set(col_i16 handle, col_i16 x, col_i16 y, char v);
extern void check_timer_tick(col_i16 n);

void mark_corner(register col_i16 x, register col_i16 y)
{
    if (x != 0 && y != 0 && (*g_map_width) > x && g_map_h > y) {
        map_handle_set(g_plane3_handle, x, y, 1);
        if ((*g_map_width) - 1 > x)
            map_handle_set(g_plane3_handle, x + 1, y, 1);
        if (g_map_h - 1 > y)
            map_handle_set(g_plane3_handle, x, y + 1, 1);
    }
    check_timer_tick(0);
}
