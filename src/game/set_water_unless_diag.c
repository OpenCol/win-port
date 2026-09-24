/* set_water_unless_diag -- shipped at 1020:047e.
 *
 * Ported by tools/port.py from matched/game/set_water_unless_diag.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short far g_plane0_handle;                    DGROUP:0x1c6a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern unsigned char map_handle_get(col_i16 handle, col_i16 x, col_i16 y);
extern void map_handle_set(col_i16 handle, col_i16 x, col_i16 y, char v);

col_i16 set_water_unless_diag(register col_i16 x, register col_i16 y)
{
    if (on_map(x, y) == 0)
        return 0;
    if (map_handle_get(g_plane0_handle, x - 1, y - 1) == 0x19)
        return 0;
    if (map_handle_get(g_plane0_handle, x - 1, y + 1) == 0x19)
        return 0;
    if (map_handle_get(g_plane0_handle, x + 1, y - 1) == 0x19)
        return 0;
    if (map_handle_get(g_plane0_handle, x + 1, y + 1) == 0x19)
        return 0;
    map_handle_set(g_plane0_handle, x, y, 0x19);
    return 1;
}
