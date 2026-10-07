/* build_map: candidate for 1020:09ea (5538 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -a
 * symbol: build_map
 * name-source: sibling-build
 * size-source: 5538, not the census 5471. The census stops at the last instruction; the 67 bytes
 * after it are the FIVE jump tables of this function's switches, and `best_defender`
 * starts immediately after the last of them. `-a` is needed for the word alignment in front of
 * them (rule 81).
 *
 * THE WHOLE NEW WORLD, generated. With a zero argument this builds a map from nothing; with a
 * non-zero one it skips straight to the tail, which is what a LOADED map needs -- the edges, the
 * region labels, the visible-ocean strip and the four starting positions.
 *
 * THE LAND comes first. Plane 0 is filled with ocean and plane 1 -- the scratch "how many strokes
 * touched this square" plane -- with zero. The brush rectangle at DGROUP:0x4bd6..0x4bd9 is set to
 * the middle of the map, three in from the left and six off one end (a coin decides WHICH end, and
 * that same coin later decides which pole gets the four rows of ocean). `elevate(0)`
 * then runs until the land count passes `((opt0 + opt1 + 1) * 8) * 40`, and as many extra strokes
 * are added as there is room for in the sixteen region slots -- fewer, at random, when the second
 * world option is up. A tidying pass then fills in every 2x2 block that is land on one diagonal
 * only, and BACKS UP a square in both directions when it does, so the fill cascades.
 *
 * THE CLIMATE is two passes. The first gives every square a latitude band: the distance from the
 * equator, jittered by `rand_range(1, 16)`, shifted by the third world option and divided by four,
 * through a switch that reads 5, 4, 1, 3, 2, 2, 0, 0 -- so the poles get terrain 0 and the tropics
 * 5. Plane 1's stroke count becomes the 0x20 and 0x80 bits, which are hills and mountains (MAP0_HILLY, MAP0_MOUNTAINS).
 *
 * The second pass is the interesting one: it sweeps each row LEFT TO RIGHT and then RIGHT TO LEFT
 * with a running "budget" seeded from the row's distance from the quarter line plus four times the
 * fourth world option. Ocean adds to the budget, mountains spend three of it, a hill is flattened (0xa0 cleared), and
 * a positive budget UPGRADES the terrain a step (with a chance of the rarer variant) while a
 * negative one downgrades it. Every step spends a random slice of the budget, which is what makes
 * terrain run in bands rather than in blocks.
 *
 * THE SCATTER PASS then pokes `opt4 * 800 + 800` points, half of them a random square and half a
 * step from the last one, and gives each terrain its own chance of growing hills (0x20) and then
 * mountains (0x80) on top -- tundra and desert 1 in 2, plains and prairie 1 in 3, grassland and
 * savannah 1 in 4, marsh and swamp 1 in 6 -- while nudging the terrain
 * itself between its neighbours. That is the pass that makes the map look hand-drawn.
 *
 * WHAT IS LEFT is trim. A land square may be turned to forest (+8, or +0x10 for the second forest band) depending on whether a
 * stroke reached it and whether it touches the sea; rivers are drawn; the poles and the east edge
 * are forced to ice and high sea; the open sea at the east edge is turned into the sea lane (up to
 * half the map in), then two passes put plain ocean (0x19) back three squares off the coast and
 * everywhere west of the lane; and the north and south rows are thinned. The tail then frames
 * the map, folds the second forest band (0x10..0x17) back onto the first and strips the forest
 * variant from hilly squares, relabels the regions, marks the
 * left-hand ocean as the Atlantic approach (bit 0x20, as in as half the map or, for a loaded
 * map, sixteen from the right) and marks every ocean square with no land within twenty ring offsets
 * as deep sea (bit 4).
 *
 * THE FOUR STARTING POSITIONS are dealt last: the four nations are shuffled into four slots -- the
 * human's own slot drawn from 1..2 on a generated map, so the player never starts at the top or the
 * bottom -- and each nation's start is the last sea-lane square before land, walking west from the east
 * edge along its own fifth of the map, with column 0x39 pulled back by one.
 *
 * WHAT THE BYTES SETTLED: the absolute value is a macro whose negative arm is `~(v) + 1`, not
 * `-(v)` -- Borland writes `NEG` for the latter and `NOT`/`INC` for the former, and the expression
 * is spelled out again inside the arm rather than held in a temporary. The first switch carries
 * `case 6:` and `case 7:` that do nothing but widen its jump table from six entries to eight. The
 * starting slot is a ternary over two `rand_range` calls, not an `if` with two stores. `t & 0x1f`
 * is cast to `unsigned char` in the last loop and left alone in the others (rule 83). And the
 * `loaded` test at the top is a `goto`, while the two range tests in the last loop are `continue`s
 * -- both read off rule 12's jump shapes.
 *
 * RENAMED 2026-09-26 from generate_map: the Mac build's build_map(short), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (callee feature_set; callee map_loc; callee terrain_is_ocean; callee on_map; callee resource_at), the runner-up gets 5, and the arity agrees (1).
 */

#include "colonization.h"

#include "terrain.h"
#include "records.h"

extern Nation g_nations[];               /* SEG20:0xc2f4 */
extern GameState g_game;   /* SEG20:0x7782 */

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern void buffer_fill(short handle, char fill_byte);
extern void map_find_continents(void);
extern unsigned char buffer_get(short handle, int x, int y);
extern void buffer_put(short handle, int x, int y, char v);
extern int build_check(int flag);
extern int water_access(int x, int y);
extern void buffer_rect_fill(short h, int x, int y, unsigned n, int rows, char c);
extern void buffer_draw_box(short h, int x, int y, int n, int rows, char c);
extern int terrain_is_ocean(int x, int y);

extern void feature_set(int x, int y, int mask, int on);
extern int resource_at(int x, int y);
extern unsigned char * map_loc(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void elevate(int pick_land);
extern void do_rivers(void);

#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)

void build_map(int loaded)
{
    int x;
    int y;
    int ax2;
    int ay;
    int flag;
    int k;
    int t;
    int t2;
    int equator_dist;
    int v;
    int n;
    int r;
    int count;
    int extra;
    int north;
    int slot;
    int nat;
    int pa;
    int pb;
    char start[4];
    register int budget;
    register int terr;

    g_scenery_seed = rand_range(1, 0x7fff);
    if (loaded != 0)
        goto tail;
    g_land_count = 0;
    buffer_fill(g_plane0_handle, 0x19);
    buffer_fill(g_plane1_handle, 0);
    g_x_lo = 3;
    g_x_hi = g_map_size.w - 6;
    g_y_lo = 0;
    g_y_hi = g_map_size.h;
    north = rand_range(0, 1);
    if (north != 0)
        g_y_lo = 5;
    else
        g_y_hi = g_map_size.h - 6;
    do {
        elevate(0);
    } while (((g_world_opt[0] + g_world_opt[1] + 1) << 3) * 0x28 > g_land_count);
    map_find_continents();
    count = 0;
    for (n = 0; n < 0x10; n++)
        if (g_region_size[n] != 0)
            count++;
    extra = 0x10 - (count + 1);
    if (extra > 0) {
        if (g_world_opt[1] > 0)
            extra = extra - rand_range(0, extra);
        for (n = 0; n < extra; n++)
            elevate(1);
    }
    for (y = 1; g_map_size.h - 1 > y; y++) {
        for (x = 1; g_map_size.w - 1 > x; x++) {
            v = 0;
            if (buffer_get(g_plane1_handle, x, y) != 0)
                v |= 1;
            if (buffer_get(g_plane1_handle, x + 1, y) != 0)
                v |= 2;
            if (buffer_get(g_plane1_handle, x, y + 1) != 0)
                v |= 4;
            if (buffer_get(g_plane1_handle, x + 1, y + 1) != 0)
                v |= 8;
            if (v == 6 || v == 9) {
                buffer_put(g_plane1_handle, x + 1, y, 1);
                buffer_put(g_plane1_handle, x, y + 1, 1);
                buffer_put(g_plane1_handle, x + 1, y + 1, 1);
                if (x != 0)
                    x--;
                if (y != 0)
                    y--;
            }
        }
    }
    build_check(0);
    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++) {
            t = buffer_get(g_plane1_handle, x, y);
            v = 0x19;
            equator_dist = ABS(((g_map_size.h >> 1) + 8) - (rand_range(1, 0x10) + y));
            equator_dist += (1 - g_world_opt[2]) * 2;
            equator_dist = max_int_1020_a887(0, equator_dist);
            equator_dist = equator_dist >> 2;
            switch (equator_dist) {
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
            buffer_put(g_plane0_handle, x, y, v);
        }
        build_check(0);
    }
    for (y = 0; g_map_size.h > y; y++) {
        equator_dist = ABS((g_map_size.h >> 1) - y);
        budget = rand_range(0, ABS((g_map_size.h >> 2) - equator_dist) + (g_world_opt[3] << 2));
        for (x = 0; g_map_size.w > x; x++) {
            t = buffer_get(g_plane0_handle, x, y);
            terr = t & 0x1f;
            if (t == 0x19) {
                if (ABS((g_map_size.h >> 2) - equator_dist) + (g_world_opt[3] << 2) > budget)
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
                        buffer_put(g_plane1_handle, x, y, 2);
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
            buffer_put(g_plane0_handle, x, y, t);
        }
        budget = 0;
        for (x = g_map_size.w - 1; x >= 0; x--) {
            t = buffer_get(g_plane0_handle, x, y);
            terr = t & 0x1f;
            if (terr == 0x19) {
                if ((equator_dist >> 1) + g_world_opt[3] > budget)
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
            buffer_put(g_plane0_handle, x, y, t);
        }
        build_check(0);
    }
    for (n = 0; g_world_opt[4] * 0x320 + 0x320 > n; n++) {
        if ((n & 1) != 0) {
            r = rand_range(0, 8);
            x += g_dir_dx[r];
            y += g_dir_dy[r];
        } else {
            x = rand_range(1, g_map_size.w - 2);
            y = rand_range(1, g_map_size.h - 2);
        }
        t = buffer_get(g_plane0_handle, x, y);
        terr = t & 0x1f;
        if ((t & 0x80) != 0) {
            if (ocean(x, y) != 0)
                t = t & 0xff5f;
        } else if ((t & 0x20) != 0) {
            t |= 0x80;
            buffer_put(g_plane1_handle, x, y, 1);
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
                    buffer_put(g_plane1_handle, x, y, 2);
                break;
            case 4:
                pa = 3;
                pb = 1;
                if (rand_range(0, 1) == 0)
                    terr = 6;
                if (rand_range(0, 1) == 0)
                    buffer_put(g_plane1_handle, x, y, 1);
                break;
            case 5:
                pa = 3;
                pb = 2;
                if (rand_range(0, 1) == 0)
                    terr = 7;
                if (rand_range(0, 1) == 0)
                    buffer_put(g_plane1_handle, x, y, 1);
                break;
            case 6:
                pa = 5;
                pb = 3;
                if (rand_range(0, 1) == 0)
                    terr = 4;
                if (rand_range(0, 1) == 0)
                    buffer_put(g_plane1_handle, x, y, 2);
                break;
            case 7:
                pa = 5;
                pb = 3;
                if (rand_range(0, 1) != 0)
                    terr = 5;
                if (rand_range(0, 1) != 0)
                    buffer_put(g_plane1_handle, x, y, 2);
                break;
            }
        }
        if (pa != 0 && rand_range(0, pa) == 0) {
            t |= 0x20;
            if (pb != 0 && rand_range(0, pb) == 0)
                t |= 0x80;
        }
        t = (t & 0xe0) | terr;
        buffer_put(g_plane0_handle, x, y, t);
    }
    build_check(0);
    for (y = 0; g_map_size.h > y; y++) {
        build_check(0);
        for (x = 0; g_map_size.w > x; x++) {
            t = buffer_get(g_plane0_handle, x, y);
            if (t != 0x19) {
                if (buffer_get(g_plane1_handle, x, y) == 1) {
                    if (rand_range(0, 8) == 0)
                        t += 8;
                    else
                        t += 0x10;
                    buffer_put(g_plane0_handle, x, y, t);
                } else {
                    if (water_access(x, y) != 0) {
                        if (rand_range(0, 1) == 0)
                            t += 8;
                        else if (rand_range(0, 4) != 0)
                            t += 0x10;
                    }
                    buffer_put(g_plane0_handle, x, y, t);
                }
            }
        }
    }
    do_rivers();
    if (north != 0)
        buffer_rect_fill(g_plane0_handle, 0, 0, g_map_size.w, 4, 0x19);
    else
        buffer_rect_fill(g_plane0_handle, 0, g_map_size.h - 4, g_map_size.w, 4, 0x19);
    buffer_draw_box(g_plane0_handle, 2, 0, g_map_size.w - 3, g_map_size.h - 1, 0x19);
    for (n = 0; n < 0x28; n++) {
        buffer_put(g_plane0_handle, rand_range(1, g_map_size.w) - 1, 1, 0x18);
        buffer_put(g_plane0_handle, rand_range(1, g_map_size.w) - 1, g_map_size.h - 2, 0x18);
    }
    for (y = 1; g_map_size.h - 1 > y; y++) {
        flag = 1;
        for (x = g_map_size.w - 1; flag != 0 && (g_map_size.w >> 1) <= x; x--) {
            if (terrain_is_ocean(x, y) != 0) {
                t = buffer_get(g_plane0_handle, x, y);
                buffer_put(g_plane0_handle, x, y, (t & 0xe0) + 0x1a);
            } else
                flag = 0;
        }
        build_check(0);
    }
    for (y = 1; g_map_size.h - 1 > y; y++) {
        flag = -1;
        for (x = g_map_size.w - 1; flag < 0 && x >= 1; x--)
            if (terrain_is_ocean(x, y) == 0)
                flag = x;
        if (flag >= 0) {
            x = flag + 3;
            x = min_int_1020_a8ad(x, g_map_size.w - 2);
            for (ay = y - 3; y + 3 >= ay; ay++) {
                if (on_map(x, ay) != 0) {
                    while (terrain_is_ocean(x, ay) == 0 && g_map_size.w - 2 > x)
                        x++;
                    if (terrain_is_ocean(x, ay) != 0) {
                        t = buffer_get(g_plane0_handle, x, ay);
                        buffer_put(g_plane0_handle, x, ay, (t & 0xe0) | 0x19);
                    }
                }
            }
        }
        build_check(0);
    }
    for (y = 1; g_map_size.h - 1 > y; y++) {
        flag = 1;
        for (x = g_map_size.w - 1; x >= 1; x--) {
            if (flag != 0) {
                if ((buffer_get(g_plane0_handle, x, y) & 0x1f) != 0x1a)
                    flag = 0;
            } else if (terrain_is_ocean(x, y) != 0) {
                t = buffer_get(g_plane0_handle, x, y);
                buffer_put(g_plane0_handle, x, y, (t & 0xe0) | 0x19);
            }
        }
        build_check(0);
    }
    for (n = 0; n < 2; n++) {
        for (x = 0; g_map_size.w > x; x++) {
            y = (n != 0) ? 1 : g_map_size.h - 2;
            if (terrain_is_ocean(x, y) == 0)
                buffer_put(g_plane0_handle, x, y, 0x18);
            y = (n != 0) ? 2 : g_map_size.h - 3;
            if (terrain_is_ocean(x, y) == 0) {
                t = terrain_at(x, y);
                t = ((rand_range(0, 1) != 0) ? 0x18 : 0) | (t & 0xe0);
                buffer_put(g_plane0_handle, x, y, t);
            }
            y = (n != 0) ? 3 : g_map_size.h - 4;
            if (rand_range(0, 1) != 0 && terrain_is_ocean(x, y) == 0) {
                t = terrain_at(x, y);
                t = t & 0xe0;
                buffer_put(g_plane0_handle, x, y, t);
            }
        }
    }
    build_check(0);
tail:
    buffer_draw_box(g_plane0_handle, 0, 0, g_map_size.w - 1, g_map_size.h - 1, 0x1a);
    buffer_draw_box(g_plane0_handle, 1, 0, g_map_size.w - 2, g_map_size.h - 1, 0x1a);
    buffer_rect_fill(g_plane0_handle, 0, 0, g_map_size.w, 1, 0x18);
    buffer_rect_fill(g_plane0_handle, 0, g_map_size.h - 1, g_map_size.w, 1, 0x18);
    for (x = 0; g_map_size.w > x; x++) {
        for (y = 0; g_map_size.h > y; y++) {
            t2 = buffer_get(g_plane0_handle, x, y);
            t = (unsigned char)(t2 & 0x1f);
            if (t >= 0x18)
                continue;
            if ((t2 & 0x20) != 0) {
                t = t & 7;
                t2 = t2 & 0xe0;
                buffer_put(g_plane0_handle, x, y, t | t2);
            } else {
                if (t < 0x10 || t >= 0x18)
                    continue;
                t = buffer_get(g_plane0_handle, x, y);
                t -= 0x10;
                t += 8;
                buffer_put(g_plane0_handle, x, y, t);
            }
        }
        build_check(0);
    }
    map_find_continents();
    buffer_fill(g_plane1_handle, 0);
    buffer_fill(g_plane3_handle, 0);
    for (y = 1; g_map_size.h - 1 > y; y++) {
        for (x = 1; ((loaded != 0) ? g_map_size.w - 0x10 : g_map_size.w >> 1) > x; x++) {
            if (terrain_is_ocean(x, y) == 0)
                break;
            feature_set(x, y, MAP1_BIT5, 1);
        }
        build_check(0);
    }
    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++) {
            if (terrain_is_ocean(x, y) == 0)
                continue;
            if (resource_at(x, y) == -1)
                continue;
            flag = 0;
            for (k = 0; flag == 0 && k < 0x14; k++) {
                ax2 = g_ring_dx[k] + x;
                ay = g_ring_dy[k] + y;
                if (on_map(ax2, ay) == 0)
                    continue;
                if (terrain_is_ocean(ax2, ay) == 0)
                    flag = 1;
            }
            if (flag == 0)
                feature_set(x, y, MAP1_DEPLETED, 1);
        }
        build_check(0);
    }
    if (loaded != 0) {
        *map_loc(0x15, 1) |= 0xa0;
        *map_loc(0x2b, 0x44) |= 0xa0;
    }
    memset_rtl(start, -1, 4);
    for (n = 0; n < 4; n++) {
        nat = (g_game.player_nation + n) % 4;
        flag = 0;
        while (flag == 0) {
            slot = (n != 0 || loaded != 0) ? rand_range(0, 3) : rand_range(1, 2);
            flag = ((int)start[slot] < 0);
        }
        start[slot] = (char)nat;
        build_check(0);
    }
    for (n = 0; n < 4; n++) {
        nat = (int)start[n];
        y = (g_map_size.h / 5) * (n + 1);
        x = g_map_size.w - 2;
        while (x > 2 && terrain_at(x, y) == TERRAIN_SEA_LANE)
            x--;
        x++;
        if (x == 0x39)
            x--;
        g_nations[nat].start_x = (char)x;
        g_nations[nat].start_y = (char)y;
    }
}
