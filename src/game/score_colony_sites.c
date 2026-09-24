/* score_colony_sites -- shipped at 1038:cbd4.
 *
 * Ported by tools/port.py from matched/game/score_colony_sites.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *   extern unsigned char far g_terrain_row_bytes[][0x10]; SEG20:0x78f0
 *   extern unsigned char far g_resource_value[];         SEG20:0xd860
 *   extern int g_map_width;                              DGROUP:0x4c40
 *   extern int g_map_h;                                  DGROUP:0x4c42
 *   extern int far g_ocean_x;                            DGROUP:0x4bc8
 *   extern int far g_ocean_y;                            DGROUP:0x4bca
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 on_map(col_i16 x, col_i16 y);
col_i16 tile_decoration(col_i16 x, col_i16 y);
unsigned char * map_plane3_ptr(col_i16 x, col_i16 y);
unsigned char map_plane0_at(col_i16 x, col_i16 y);
unsigned char map_plane2_nibble(col_i16 x, col_i16 y);

#include "terrain.h"










extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern col_i16 find_adjacent_ocean(col_i16 x, col_i16 y);
extern col_i16 minimax(col_i16 v, col_i16 lo, col_i16 hi);
extern col_i16 check_timer_tick(col_i16 flag);

/* same object: 4-byte `push cs; call ` */






void score_colony_sites(void)
{
    col_i16 x;
    col_i16 y;
    col_i16 nx;
    col_i16 ny;
    col_i16 i;
    col_i16 ax2;
    col_i16 ay2;
    col_i16 water;
    col_i16 v;
    col_i16 terr;
    col_i16 dec;
    col_i16 total;
    col_i16 adj;
    register col_i16 d;
    register col_i16 w;

    for (y = 0; y < g_map_h; y++) {
        for (x = 0; x < (*g_map_width); x++) {
            total = 0;
            if (on_map(x, y) != 0 && ocean(x, y) == 0) {
                for (i = 0; i < 0x15; i++) {
                    nx = g_ring_dx[i] + x;
                    ny = g_ring_dy[i] + y;
                    v = 0;
                    if (on_map(nx, ny) != 0) {
                        terr = terrain_at(nx, ny);
                        dec = tile_decoration(nx, ny);
                        if (dec != -1) {
                            v += g_resource_value[dec];
                        } else if (terr == TERRAIN_OCEAN) {
                            water = 2;
                            for (d = 0; d < 8; d++) {
                                ax2 = g_dir_dx[d] + nx;
                                ay2 = g_dir_dy[d] + ny;
                                if (ocean(ax2, ay2) == 0)
                                    water += 2;
                            }
                            v = water >> 2;
                        } else {
                            v += g_terrain_row_bytes[terr][0xd];
                        }
                        if (map_plane0_at(nx, ny) & MAP0_RIVER)
                            v++;
                        w = 2;
                        if (i < 4) {
                            w += 3;
                            if (terr == TERRAIN_OCEAN)
                                v = (v + 1) >> 1;
                        }
                        if (i < 8)
                            w += 2;
                        if (i < 0xc)
                            w++;
                        if (i < 0x14)
                            w++;
                        if (i == 0x14)
                            w = 4;
                        v = v * w >> 1;
                        total = total + v;
                    }
                }
            }
            adj = find_adjacent_ocean(x, y);
            if (adj != 0 && map_plane2_nibble(g_ocean_x, g_ocean_y) != 1)
                adj = 0;
            if (adj == 0)
                total = total >> 1;
            if (terrain_at(x, y) == TERRAIN_MOUNTAINS)
                total = 0;
            if (terrain_at(x, y) == TERRAIN_HILLS)
                total = total >> 1;
            v = minimax(total / 10, 0, 0xf);
            *map_plane3_ptr(x, y) = v;
        }
        check_timer_tick(0);
    }
}
