/* walk_orthogonal -- shipped at 1020:026d.
 *
 * Ported by tools/port.py from matched/game/walk_orthogonal.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short far g_plane3_handle;                    DGROUP:0x1c70
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern signed char g_x_lo;                           DGROUP:0x4bd6
 *   extern signed char g_y_lo;                           DGROUP:0x4bd7
 *   extern signed char g_x_hi;                           DGROUP:0x4bd8
 *   extern signed char g_y_hi;                           DGROUP:0x4bd9
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void map_handle_set(col_i16 handle, col_i16 x, col_i16 y, char v);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void check_timer_tick(col_i16 n);

void walk_orthogonal(register col_i16 x, register col_i16 y)
{
    col_i16 n;
    col_i16 d;

    n = rand_range(1, 0x10) + 2;
    while (n-- != 0 && g_x_lo < x && g_x_hi > x && g_y_lo < y && g_y_hi > y) {
        map_handle_set(g_plane3_handle, x, y, 1);
        d = rand_range(1, 4) * 2 - 2;
        x += g_dir_dx[d];
        y += g_dir_dy[d];
    }
    check_timer_tick(0);
}
