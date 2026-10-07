/* map_gen_rivers -- shipped at 1020:0563.
 *
 * Ported by tools/port.py from matched/game/map_gen_rivers.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_plane0_handle;                      DGROUP:0x1c6a
 *   extern int far g_plane3_handle;                      DGROUP:0x1c70
 *   extern unsigned char huge *far g_plane0;             DGROUP:0x1c72
 *   extern unsigned char huge *far g_plane3;             DGROUP:0x1c7e
 *   extern unsigned far g_map_bytes;                     DGROUP:0x1c96
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern int far g_world_opt[4];                       SEG22:0x0000
 *   extern signed char far g_ortho_dx[];                 DGROUP:0x1c58
 *   extern signed char far g_ortho_dy[];                 DGROUP:0x1c5d
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * memcpy_rtl(void *d, const void *s, col_u16 n);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern unsigned char map_handle_get(col_i16 h, col_i16 x, col_i16 y);
extern void map_handle_set(col_i16 h, col_i16 x, col_i16 y, col_i16 v);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern void check_timer_tick(col_i16 a);

void map_gen_rivers(void)
{
    col_i16 rivers;
    col_i16 tries;
    col_i16 y;
    col_i16 v;
    col_i16 dir;
    col_i16 r;
    col_i16 nd;
    col_i16 joined;
    col_i16 nx;
    col_i16 ny;
    col_i16 mx;
    col_i16 my;
    col_i16 sx;
    col_i16 sy;
    col_i16 len;
    col_i16 len2;
    col_i16 turn;
    register col_i16 i;
    register col_i16 x;

    rivers = 0;
    tries = 0;
    do {
        memcpy_rtl(g_plane3, g_plane0, g_map_bytes);
        tries++;
        len = 0;
        do {
            x = rand_range(1, (*g_map_width) - 2);
            y = rand_range(1, g_map_h - 2);
            v = map_handle_get(g_plane3_handle, x, y);
        } while ((v & 0x20) || ocean(x, y));
        sx = x;
        sy = y;
        dir = rand_range(0, 3) * 2;
        turn = rand_range(0, 1);
        do {
            v |= 0x40;
            map_handle_set(g_plane0_handle, x, y, v);
            len++;
            joined = 0;
            for (i = 0; joined == 0 && i < 4; i++) {
                nx = g_ortho_dx[i] + x;
                ny = g_ortho_dy[i] + y;
                if (ocean(nx, ny) || (map_handle_get(g_plane3_handle, nx, ny) & 0x40)) {
                    joined = 1;
                    v = map_handle_get(g_plane0_handle, nx, ny);
                    v |= 0x40;
                    map_handle_set(g_plane0_handle, nx, ny, v);
                    mx = x;
                    my = y;
                }
            }
            r = rand_range(0, 99);
            if (r < 60)
                nd = dir;
            else {
                if (r > 95)
                    turn = !turn;
                if (turn)
                    nd = (dir + 2) % 8;
                else
                    nd = (dir + 6) % 8;
                turn = !turn;
                (dir + 6) % 8;
            }
            x = g_dir_dx[nd] + x;
            y = g_dir_dy[nd] + y;
            dir = nd;
            v = map_handle_get(g_plane3_handle, x, y);
        } while (joined == 0 && on_map(x, y) && (v & 0x40) == 0 && (v & 0x20) == 0);
        if ((joined == 0 && (v & 0x40) == 0) || len < 3) {
            memcpy_rtl(g_plane0, g_plane3, g_map_bytes);
        } else {
            rivers++;
            if (joined != 0 && rand_range(1, g_world_opt[3] * 2 + 12) > 6) {
                len2 = rand_range(1, g_world_opt[3] * 2 + 3);
                v = map_handle_get(g_plane0_handle, mx, my);
                do {
                    v |= 0x80;
                    map_handle_set(g_plane0_handle, mx, my, v);
                    nd = -1;
                    for (i = 0; nd < 0 && i < 4; i++) {
                        x = g_ortho_dx[i] + mx;
                        y = g_ortho_dy[i] + my;
                        v = map_handle_get(g_plane0_handle, x, y);
                        if ((v & 0x40) && (v & 0x80) == 0) {
                            nd = i;
                            mx = x;
                            my = y;
                            break;
                        }
                    }
                    len2--;
                } while (len2 > 0 && nd >= 0);
            }
            for (i = 0; i < 20; i++) {
                x = g_ring_dx[i] + sx;
                y = g_ring_dy[i] + sy;
                v = map_handle_get(g_plane0_handle, x, y);
                if ((unsigned char)(v & 0x1f) < 0x10 && rand_range(0, 1)) {
                    v += 8;
                    map_handle_set(g_plane0_handle, x, y, v);
                }
            }
        }
        check_timer_tick(0);
    } while (tries < 0x200 && ((g_world_opt[0] << 2) + (g_world_opt[3] << 2) + 8) << 1 > rivers);
}
