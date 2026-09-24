/* generate_map -- shipped at 1020:09ea.
 *
 * Ported by tools/port.py from matched/game/generate_map.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far g_nations[];                       SEG20:0xc2f4
 *   extern int far g_player_nation;                      SEG20:0x779a
 *   extern int far g_world_opt[];                        SEG22:0x0000
 *   extern int far g_region_size[];                      SEG27:0x008c
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern signed char far g_ring_dx[];                  SEG27:0x0012
 *   extern signed char far g_ring_dy[];                  SEG27:0x0027
 *   extern short far g_plane0_handle;                    DGROUP:0x1c6a
 *   extern short far g_plane1_handle;                    DGROUP:0x1c6c
 *   extern short far g_plane3_handle;                    DGROUP:0x1c70
 *   extern int far g_scenery_seed;                       DGROUP:0x1ca6
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern int g_land_count;                             DGROUP:0x4bd4
 *   extern signed char g_x_lo;                           DGROUP:0x4bd6
 *   extern signed char g_y_lo;                           DGROUP:0x4bd7
 *   extern signed char g_x_hi;                           DGROUP:0x4bd8
 *   extern signed char g_y_hi;                           DGROUP:0x4bd9
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1020_a887(col_i16 a, col_i16 b);
col_i16 min_int_1020_a8ad(col_i16 a, col_i16 b);
col_i16 set_water_unless_diag(col_i16 x, col_i16 y);
void grow_landmass(col_i16 pick_land);
void map_gen_rivers(void);

#include "terrain.h"
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void handle_fill_bytes(col_i16 handle, char c);
extern void label_map_regions(void);
extern unsigned char map_handle_get(col_i16 handle, col_i16 x, col_i16 y);
extern void map_handle_set(col_i16 handle, col_i16 x, col_i16 y, char v);
extern void check_timer_tick(col_i16 n);
extern col_i16 find_adjacent_ocean(col_i16 x, col_i16 y);
extern void map_handle_fill_rows(col_i16 h, col_i16 x, col_i16 y, col_u16 n, col_i16 rows, char c);
extern void map_handle_frame(col_i16 h, col_i16 x, col_i16 y, col_i16 n, col_i16 rows, char c);
extern col_i16 ocean(col_i16 x, col_i16 y);
extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 terrain_at(col_i16 x, col_i16 y);
extern void map_plane1_set_bits(col_i16 x, col_i16 y, unsigned char mask, col_i16 on);
extern col_i16 tile_decoration(col_i16 x, col_i16 y);
extern unsigned char * map_plane0_ptr(col_i16 x, col_i16 y);
extern void * memset_rtl(void *p, col_i16 c, col_u16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */






#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)

void generate_map(col_i16 loaded)
{
    col_i16 x;
    col_i16 y;
    col_i16 ax2;
    col_i16 ay;
    col_i16 flag;
    col_i16 k;
    col_i16 t;
    col_i16 t2;
    col_i16 d;
    col_i16 v;
    col_i16 n;
    col_i16 r;
    col_i16 count;
    col_i16 extra;
    col_i16 north;
    col_i16 slot;
    col_i16 nat;
    col_i16 pa;
    col_i16 pb;
    char start[4];
    register col_i16 budget;
    register col_i16 terr;

    g_scenery_seed = rand_range(1, 0x7fff);
    if (loaded != 0)
        goto tail;
    g_land_count = 0;
    handle_fill_bytes(g_plane0_handle, 0x19);
    handle_fill_bytes(g_plane1_handle, 0);
    g_x_lo = 3;
    g_x_hi = (*g_map_width) - 6;
    g_y_lo = 0;
    g_y_hi = g_map_h;
    north = rand_range(0, 1);
    if (north != 0)
        g_y_lo = 5;
    else
        g_y_hi = g_map_h - 6;
    do {
        grow_landmass(0);
    } while (((g_world_opt[0] + g_world_opt[1] + 1) << 3) * 0x28 > g_land_count);
    label_map_regions();
    count = 0;
    for (n = 0; n < 0x10; n++)
        if (g_region_size[n] != 0)
            count++;
    extra = 0x10 - (count + 1);
    if (extra > 0) {
        if (g_world_opt[1] > 0)
            extra = extra - rand_range(0, extra);
        for (n = 0; n < extra; n++)
            grow_landmass(1);
    }
    for (y = 1; g_map_h - 1 > y; y++) {
        for (x = 1; (*g_map_width) - 1 > x; x++) {
            v = 0;
            if (map_handle_get(g_plane1_handle, x, y) != 0)
                v |= 1;
            if (map_handle_get(g_plane1_handle, x + 1, y) != 0)
                v |= 2;
            if (map_handle_get(g_plane1_handle, x, y + 1) != 0)
                v |= 4;
            if (map_handle_get(g_plane1_handle, x + 1, y + 1) != 0)
                v |= 8;
            if (v == 6 || v == 9) {
                map_handle_set(g_plane1_handle, x + 1, y, 1);
                map_handle_set(g_plane1_handle, x, y + 1, 1);
                map_handle_set(g_plane1_handle, x + 1, y + 1, 1);
                if (x != 0)
                    x--;
                if (y != 0)
                    y--;
            }
        }
    }
    check_timer_tick(0);
    for (y = 0; g_map_h > y; y++) {
        for (x = 0; (*g_map_width) > x; x++) {
            t = map_handle_get(g_plane1_handle, x, y);
            v = 0x19;
            d = ABS(((g_map_h >> 1) + 8) - (rand_range(1, 0x10) + y));
            d += (1 - g_world_opt[2]) * 2;
            d = max_int_1020_a887(0, d);
            d = d >> 2;
            switch (d) {
            case 0:
                v = 5;
                break;
            case 1:
                v = 4;
                break;
            case 2:
                v = 1;
                break;
            case 3:
                v = 3;
                break;
            case 4:
            case 5:
                v = 2;
                break;
            case 6:
            case 7:
            default:
                v = 0;
                break;
            }
            if (t == 0)
                v = 0x19;
            if (t >= 2)
                v |= 0x20;
            if (t >= 3)
                v |= 0x80;
            map_handle_set(g_plane0_handle, x, y, v);
        }
        check_timer_tick(0);
    }
    for (y = 0; g_map_h > y; y++) {
        d = ABS((g_map_h >> 1) - y);
        budget = rand_range(0, ABS((g_map_h >> 2) - d) + (g_world_opt[3] << 2));
        for (x = 0; (*g_map_width) > x; x++) {
            t = map_handle_get(g_plane0_handle, x, y);
            terr = t & 0x1f;
            if (t == 0x19) {
                if (ABS((g_map_h >> 2) - d) + (g_world_opt[3] << 2) > budget)
                    budget++;
            } else {
                if ((t & 0x80) != 0)
                    budget -= 3;
                else if ((t & 0x20) != 0)
                    t = t & 0xff5f;
                else if (budget < 0) {
                    switch (terr) {
                    case 4:
                        terr = 3;
                        break;
                    case 3:
                        if (rand_range(0, ABS(budget)) != 0)
                            terr = 1;
                        else {
                            terr = 2;
                            budget--;
                        }
                        break;
                    case 2:
                        terr = 0;
                        break;
                    case 0:
                        map_handle_set(g_plane1_handle, x, y, 2);
                        break;
                    }
                } else if (budget > 0) {
                    switch (terr) {
                    case 5:
                        budget -= 2;
                        if (rand_range(0, 3) == 0)
                            terr = 7;
                        break;
                    case 4:
                        budget -= 2;
                        if (rand_range(0, 3) == 0)
                            terr = 6;
                        break;
                    case 3:
                        terr = 4;
                        break;
                    case 2:
                        terr = 3;
                        break;
                    case 0:
                        terr = 2;
                        break;
                    }
                }
                if (budget > 0)
                    budget -= rand_range(1, 7 - g_world_opt[3] * 2);
                else if (budget < 0)
                    budget++;
            }
            t = (t & 0xe0) | terr;
            map_handle_set(g_plane0_handle, x, y, t);
        }
        budget = 0;
        for (x = (*g_map_width) - 1; x >= 0; x--) {
            t = map_handle_get(g_plane0_handle, x, y);
            terr = t & 0x1f;
            if (terr == 0x19) {
                if ((d >> 1) + g_world_opt[3] > budget)
                    budget++;
            } else {
                if ((t & 0x80) != 0)
                    budget -= 3;
                else if ((t & 0x20) != 0)
                    t = t & 0xff5f;
                else if (budget < 0) {
                } else if (budget > 0) {
                    switch (terr) {
                    case 5:
                        budget -= 2;
                        terr = 7;
                        break;
                    case 4:
                        budget -= 2;
                        if (rand_range(0, 1) == 0)
                            terr = 6;
                        break;
                    case 3:
                        terr = 4;
                        break;
                    case 2:
                        terr = 3;
                        break;
                    case 1:
                        terr = 3;
                        break;
                    case 0:
                        terr = 2;
                        break;
                    }
                }
                if (budget > 0)
                    budget -= rand_range(1, 7 - g_world_opt[3] * 2);
                else if (budget < 0)
                    budget++;
            }
            t = (t & 0xe0) | terr;
            map_handle_set(g_plane0_handle, x, y, t);
        }
        check_timer_tick(0);
    }
    for (n = 0; g_world_opt[4] * 0x320 + 0x320 > n; n++) {
        if ((n & 1) != 0) {
            r = rand_range(0, 8);
            x += g_dir_dx[r];
            y += g_dir_dy[r];
        } else {
            x = rand_range(1, (*g_map_width) - 2);
            y = rand_range(1, g_map_h - 2);
        }
        t = map_handle_get(g_plane0_handle, x, y);
        terr = t & 0x1f;
        if ((t & 0x80) != 0) {
            if (set_water_unless_diag(x, y) != 0)
                t = t & 0xff5f;
        } else if ((t & 0x20) != 0) {
            t |= 0x80;
            map_handle_set(g_plane1_handle, x, y, 1);
        } else {
            pa = 0;
            pb = 0;
            switch (terr) {
            case 0:
                pa = 1;
                pb = 0;
                if (rand_range(0, 1) == 0)
                    terr = 2;
                break;
            case 1:
                pa = 1;
                pb = 1;
                if (rand_range(0, 1) == 0)
                    terr = 3;
                break;
            case 2:
            case 3:
                if (terr == 3 && rand_range(0, 2) == 0)
                    terr = 2;
                pa = pb = 2;
                if (rand_range(0, 1) == 0)
                    map_handle_set(g_plane1_handle, x, y, 2);
                break;
            case 4:
                pa = 3;
                pb = 1;
                if (rand_range(0, 1) == 0)
                    terr = 6;
                if (rand_range(0, 1) == 0)
                    map_handle_set(g_plane1_handle, x, y, 1);
                break;
            case 5:
                pa = 3;
                pb = 2;
                if (rand_range(0, 1) == 0)
                    terr = 7;
                if (rand_range(0, 1) == 0)
                    map_handle_set(g_plane1_handle, x, y, 1);
                break;
            case 6:
                pa = 5;
                pb = 3;
                if (rand_range(0, 1) == 0)
                    terr = 4;
                if (rand_range(0, 1) == 0)
                    map_handle_set(g_plane1_handle, x, y, 2);
                break;
            case 7:
                pa = 5;
                pb = 3;
                if (rand_range(0, 1) != 0)
                    terr = 5;
                if (rand_range(0, 1) != 0)
                    map_handle_set(g_plane1_handle, x, y, 2);
                break;
            }
        }
        if (pa != 0 && rand_range(0, pa) == 0) {
            t |= 0x20;
            if (pb != 0 && rand_range(0, pb) == 0)
                t |= 0x80;
        }
        t = (t & 0xe0) | terr;
        map_handle_set(g_plane0_handle, x, y, t);
    }
    check_timer_tick(0);
    for (y = 0; g_map_h > y; y++) {
        check_timer_tick(0);
        for (x = 0; (*g_map_width) > x; x++) {
            t = map_handle_get(g_plane0_handle, x, y);
            if (t != 0x19) {
                if (map_handle_get(g_plane1_handle, x, y) == 1) {
                    if (rand_range(0, 8) == 0)
                        t += 8;
                    else
                        t += 0x10;
                    map_handle_set(g_plane0_handle, x, y, t);
                } else {
                    if (find_adjacent_ocean(x, y) != 0) {
                        if (rand_range(0, 1) == 0)
                            t += 8;
                        else if (rand_range(0, 4) != 0)
                            t += 0x10;
                    }
                    map_handle_set(g_plane0_handle, x, y, t);
                }
            }
        }
    }
    map_gen_rivers();
    if (north != 0)
        map_handle_fill_rows(g_plane0_handle, 0, 0, (*g_map_width), 4, 0x19);
    else
        map_handle_fill_rows(g_plane0_handle, 0, g_map_h - 4, (*g_map_width), 4, 0x19);
    map_handle_frame(g_plane0_handle, 2, 0, (*g_map_width) - 3, g_map_h - 1, 0x19);
    for (n = 0; n < 0x28; n++) {
        map_handle_set(g_plane0_handle, rand_range(1, (*g_map_width)) - 1, 1, 0x18);
        map_handle_set(g_plane0_handle, rand_range(1, (*g_map_width)) - 1, g_map_h - 2, 0x18);
    }
    for (y = 1; g_map_h - 1 > y; y++) {
        flag = 1;
        for (x = (*g_map_width) - 1; flag != 0 && ((*g_map_width) >> 1) <= x; x--) {
            if (ocean(x, y) != 0) {
                t = map_handle_get(g_plane0_handle, x, y);
                map_handle_set(g_plane0_handle, x, y, (t & 0xe0) + 0x1a);
            } else
                flag = 0;
        }
        check_timer_tick(0);
    }
    for (y = 1; g_map_h - 1 > y; y++) {
        flag = -1;
        for (x = (*g_map_width) - 1; flag < 0 && x >= 1; x--)
            if (ocean(x, y) == 0)
                flag = x;
        if (flag >= 0) {
            x = flag + 3;
            x = min_int_1020_a8ad(x, (*g_map_width) - 2);
            for (ay = y - 3; y + 3 >= ay; ay++) {
                if (on_map(x, ay) != 0) {
                    while (ocean(x, ay) == 0 && (*g_map_width) - 2 > x)
                        x++;
                    if (ocean(x, ay) != 0) {
                        t = map_handle_get(g_plane0_handle, x, ay);
                        map_handle_set(g_plane0_handle, x, ay, (t & 0xe0) | 0x19);
                    }
                }
            }
        }
        check_timer_tick(0);
    }
    for (y = 1; g_map_h - 1 > y; y++) {
        flag = 1;
        for (x = (*g_map_width) - 1; x >= 1; x--) {
            if (flag != 0) {
                if ((map_handle_get(g_plane0_handle, x, y) & 0x1f) != 0x1a)
                    flag = 0;
            } else if (ocean(x, y) != 0) {
                t = map_handle_get(g_plane0_handle, x, y);
                map_handle_set(g_plane0_handle, x, y, (t & 0xe0) | 0x19);
            }
        }
        check_timer_tick(0);
    }
    for (n = 0; n < 2; n++) {
        for (x = 0; (*g_map_width) > x; x++) {
            y = (n != 0) ? 1 : g_map_h - 2;
            if (ocean(x, y) == 0)
                map_handle_set(g_plane0_handle, x, y, 0x18);
            y = (n != 0) ? 2 : g_map_h - 3;
            if (ocean(x, y) == 0) {
                t = terrain_at(x, y);
                t = ((rand_range(0, 1) != 0) ? 0x18 : 0) | (t & 0xe0);
                map_handle_set(g_plane0_handle, x, y, t);
            }
            y = (n != 0) ? 3 : g_map_h - 4;
            if (rand_range(0, 1) != 0 && ocean(x, y) == 0) {
                t = terrain_at(x, y);
                t = t & 0xe0;
                map_handle_set(g_plane0_handle, x, y, t);
            }
        }
    }
    check_timer_tick(0);
tail:
    map_handle_frame(g_plane0_handle, 0, 0, (*g_map_width) - 1, g_map_h - 1, 0x1a);
    map_handle_frame(g_plane0_handle, 1, 0, (*g_map_width) - 2, g_map_h - 1, 0x1a);
    map_handle_fill_rows(g_plane0_handle, 0, 0, (*g_map_width), 1, 0x18);
    map_handle_fill_rows(g_plane0_handle, 0, g_map_h - 1, (*g_map_width), 1, 0x18);
    for (x = 0; (*g_map_width) > x; x++) {
        for (y = 0; g_map_h > y; y++) {
            t2 = map_handle_get(g_plane0_handle, x, y);
            t = (unsigned char)(t2 & 0x1f);
            if (t >= 0x18)
                continue;
            if ((t2 & 0x20) != 0) {
                t = t & 7;
                t2 = t2 & 0xe0;
                map_handle_set(g_plane0_handle, x, y, t | t2);
            } else {
                if (t < 0x10 || t >= 0x18)
                    continue;
                t = map_handle_get(g_plane0_handle, x, y);
                t -= 0x10;
                t += 8;
                map_handle_set(g_plane0_handle, x, y, t);
            }
        }
        check_timer_tick(0);
    }
    label_map_regions();
    handle_fill_bytes(g_plane1_handle, 0);
    handle_fill_bytes(g_plane3_handle, 0);
    for (y = 1; g_map_h - 1 > y; y++) {
        for (x = 1; ((loaded != 0) ? (*g_map_width) - 0x10 : (*g_map_width) >> 1) > x; x++) {
            if (ocean(x, y) == 0)
                break;
            map_plane1_set_bits(x, y, MAP1_BIT5, 1);
        }
        check_timer_tick(0);
    }
    for (y = 0; g_map_h > y; y++) {
        for (x = 0; (*g_map_width) > x; x++) {
            if (ocean(x, y) == 0)
                continue;
            if (tile_decoration(x, y) == -1)
                continue;
            flag = 0;
            for (k = 0; flag == 0 && k < 0x14; k++) {
                ax2 = g_ring_dx[k] + x;
                ay = g_ring_dy[k] + y;
                if (on_map(ax2, ay) == 0)
                    continue;
                if (ocean(ax2, ay) == 0)
                    flag = 1;
            }
            if (flag == 0)
                map_plane1_set_bits(x, y, MAP1_DEPLETED, 1);
        }
        check_timer_tick(0);
    }
    if (loaded != 0) {
        *map_plane0_ptr(0x15, 1) |= 0xa0;
        *map_plane0_ptr(0x2b, 0x44) |= 0xa0;
    }
    memset_rtl(start, -1, 4);
    for (n = 0; n < 4; n++) {
        nat = (g_player_nation + n) % 4;
        flag = 0;
        while (flag == 0) {
            slot = (n != 0 || loaded != 0) ? rand_range(0, 3) : rand_range(1, 2);
            flag = ((col_i16)start[slot] < 0);
        }
        start[slot] = (char)nat;
        check_timer_tick(0);
    }
    for (n = 0; n < 4; n++) {
        nat = (col_i16)start[n];
        y = (g_map_h / 5) * (n + 1);
        x = (*g_map_width) - 2;
        while (x > 2 && terrain_at(x, y) == TERRAIN_SEA_LANE)
            x--;
        x++;
        if (x == 0x39)
            x--;
        g_nations[nat].start_x = (char)x;
        g_nations[nat].start_y = (char)y;
    }
}
