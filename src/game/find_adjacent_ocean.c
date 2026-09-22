/* find_adjacent_ocean -- shipped at 1018:39e3.
 *
 * Ported by tools/port.py from matched/game/find_adjacent_ocean.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern int g_ocean_x;                                DGROUP:0x4bc8
 *   extern int g_ocean_y;                                DGROUP:0x4bca
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern unsigned char map_plane2_nibble(col_i16 x, col_i16 y);

col_i16 find_adjacent_ocean(col_i16 x, col_i16 y)
{
    register col_i16 dir;
    register col_i16 nx;
    col_i16 found = 0;
    col_i16 ny;
    col_i16 v;
    col_i16 best = -1;

    for (dir = 0; dir < 8; dir++) {
        nx = g_dir_dx[dir] + x;
        ny = g_dir_dy[dir] + y;
        if (on_map(nx, ny) != 0 && ocean(nx, ny) != 0) {
            found = 1;
            v = map_plane2_nibble(nx, ny);
            if (v == 0)
                v = 0x10;
            if (best < 0 || v < best) {
                best = v;
                g_ocean_x = nx;
                g_ocean_y = ny;
            }
        }
    }
    return found;
}
