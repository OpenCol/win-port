/* do_rivers: candidate for 1020:0563 (1159 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE MAP GENERATOR'S RIVERS, one of the passes `1020:09ea` runs. Rivers are drawn into plane 0 of
 * the map (bits 0x40 river, 0x80 major river) until there are `2 x (4 x world option 0 + 4 x world
 * option 3 + 8)` of them, or 512 attempts have been made.
 *
 * ONE ATTEMPT. Plane 0 is first copied to plane 3, which serves as scratch here. A river starts on a
 * random inland land square that is not hilly (plane-0 bit 0x20) and heads in one of the four even
 * directions (0, 2, 4 or 6 of the eight-direction table), with a random turning preference. Each step marks the square as
 * river and looks at its four orthogonal neighbours: water, or a river that was already there before this
 * attempt, JOINS the river there (and marks that neighbour). Otherwise it moves on: straight ahead 60% of
 * the time, and otherwise turning 90 degrees to the preferred side. The preference flips after every
 * turn, and also on a roll above 95. The walk ends at a join, off the map, or on hills or an older river.
 *
 * KEPT OR UNDONE. A river of fewer than 3 squares, or one that ended without joining anything, is undone
 * by copying plane 3 back. A kept river that joined something may become a MAJOR river upstream from
 * its mouth (`rand(1, 2 x option 3 + 12) > 6`: an even chance at option 0, better with a higher world option 3): the major bit follows the river
 * back for up to `2 x option 3 + 3` squares. And each of the 20 squares around the river's source
 * whose terrain id is below 16 (the open kinds) turns to its forest (id + 8) on a coin flip.
 *
 * Two expression spellings the bytes fixed:
 *   - the attempt test is written `(... + 8) << 1`; the `* 2` spelling folds the 8 into a 16;
 *   - a dead `(dir + 6) % 8;` statement after a turn is compiled in the shipped, and it is kept here.
 * The byte tests are `(v & bit) == 0` (in place).
 *
 * RENAMED 2026-09-26 from map_gen_rivers: the Mac build's do_rivers(void), bound by call-sequence alignment (1.0 aligned gaps in build_map leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.94x this one.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern unsigned char buffer_get(short handle, int x, int y);
extern void buffer_put(short handle, int x, int y, char value);
extern int terrain_is_ocean(int x, int y);

extern int build_check(int flag);

void do_rivers(void)
{
    int rivers;
    int tries;
    int y;
    int v;
    int dir;
    int r;
    int nd;
    int joined;
    int nx;
    int ny;
    int mx;
    int my;
    int sx;
    int sy;
    int len;
    int len2;
    int turn;
    register int i;
    register int x;

    rivers = 0;
    tries = 0;
    do {
        memcpy_rtl(g_plane3, g_plane0, g_map_bytes);
        tries++;
        len = 0;
        do {
            x = rand_range(1, g_map_size.w - 2);
            y = rand_range(1, g_map_size.h - 2);
            v = buffer_get(g_plane3_handle, x, y);
        } while ((v & 0x20) || terrain_is_ocean(x, y));
        sx = x;
        sy = y;
        dir = rand_range(0, 3) * 2;
        turn = rand_range(0, 1);
        do {
            v |= 0x40;
            buffer_put(g_plane0_handle, x, y, v);
            len++;
            joined = 0;
            for (i = 0; joined == 0 && i < 4; i++) {
                nx = g_ortho_dx[i] + x;
                ny = g_ortho_dy[i] + y;
                if (terrain_is_ocean(nx, ny) || (buffer_get(g_plane3_handle, nx, ny) & 0x40)) {
                    joined = 1;
                    v = buffer_get(g_plane0_handle, nx, ny);
                    v |= 0x40;
                    buffer_put(g_plane0_handle, nx, ny, v);
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
            v = buffer_get(g_plane3_handle, x, y);
        } while (joined == 0 && on_map(x, y) && (v & 0x40) == 0 && (v & 0x20) == 0);
        if ((joined == 0 && (v & 0x40) == 0) || len < 3) {
            memcpy_rtl(g_plane0, g_plane3, g_map_bytes);
        } else {
            rivers++;
            if (joined != 0 && rand_range(1, g_world_opt[3] * 2 + 12) > 6) {
                len2 = rand_range(1, g_world_opt[3] * 2 + 3);
                v = buffer_get(g_plane0_handle, mx, my);
                do {
                    v |= 0x80;
                    buffer_put(g_plane0_handle, mx, my, v);
                    nd = -1;
                    for (i = 0; nd < 0 && i < 4; i++) {
                        x = g_ortho_dx[i] + mx;
                        y = g_ortho_dy[i] + my;
                        v = buffer_get(g_plane0_handle, x, y);
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
                v = buffer_get(g_plane0_handle, x, y);
                if ((unsigned char)(v & 0x1f) < 0x10 && rand_range(0, 1)) {
                    v += 8;
                    buffer_put(g_plane0_handle, x, y, v);
                }
            }
        }
        build_check(0);
    } while (tries < 0x200 && ((g_world_opt[0] << 2) + (g_world_opt[3] << 2) + 8) << 1 > rivers);
}
