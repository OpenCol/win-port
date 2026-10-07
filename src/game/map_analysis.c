/* map_analysis: candidate for 1038:cbd4 (642 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * SCORES EVERY LAND SQUARE AS A COLONY SITE and writes the result into map plane 3, a map
 * generator step (the timer is pumped once per row). Water and off-map squares score 0.
 *
 * A square's score is the sum over its own 21-square ring of a per-square value, each weighted
 * by how close it is: 9 for the middle four, then 6, 4 and 3 further out, and 4 for the last
 * ring entry -- then halved. A square's value is the decoration's byte at SEG20:0xd860 if it
 * carries one; for open water, 2 plus 2 per non-water neighbour, quartered; otherwise the
 * terrain's byte at +0xd of its 16-byte row. A bonus point for plane-0 bit 6, and water in the
 * middle four counts half, rounded up.
 *
 * The whole is halved unless the square touches ocean whose plane-2 nibble is 1 -- a sea route,
 * not a lake -- zeroed on terrain 0x1b and halved on 0x1c, then divided by ten and clamped to
 * 0..15, which is what fits in the plane-3 nibble.
 *
 * `minimax`, `build_check`, `terrain_is_ocean`, `terrain_at` and `water_access` are extern; the other five
 * helpers come through the 4-byte `PUSH CS` form and are stubs here (rule 47). `v += ...`
 * accumulates in place where `total = total + v` loads and stores -- both are in the shipped,
 * three instructions apart (rule 60).
 *
 * RENAMED 2026-09-26 from score_colony_sites: the Mac build's map_analysis(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee minimax; callee terrain_at; callee water_access), the runner-up gets 2, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "terrain.h"

#include "records.h"

extern MapSize g_map_size;   /* DGROUP:0x4c40 */

extern int terrain_is_ocean(int x, int y);

extern int water_access(int x, int y);

extern int build_check(int flag);

/* same object: 4-byte `push cs; call ` */

extern int resource_at(int x, int y);
extern unsigned char map_get(int x, int y);
extern unsigned char continent_at(int x, int y);
extern unsigned char *site_loc(int x, int y);

void map_analysis(void)
{
    int x;
    int y;
    int nx;
    int ny;
    int i;
    int ax2;
    int ay2;
    int water;
    int value;
    int terr;
    int dec;
    int total;
    int adj;
    register int dir;
    register int w;

    for (y = 0; y < g_map_size.h; y++) {
        for (x = 0; x < g_map_size.w; x++) {
            total = 0;
            if (on_map(x, y) != 0 && terrain_is_ocean(x, y) == 0) {
                for (i = 0; i < 0x15; i++) {
                    nx = g_ring_dx[i] + x;
                    ny = g_ring_dy[i] + y;
                    value = 0;
                    if (on_map(nx, ny) != 0) {
                        terr = terrain_at(nx, ny);
                        dec = resource_at(nx, ny);
                        if (dec != -1) {
                            value += g_resource_value[dec];
                        } else if (terr == TERRAIN_OCEAN) {
                            water = 2;
                            for (dir = 0; dir < 8; dir++) {
                                ax2 = g_dir_dx[dir] + nx;
                                ay2 = g_dir_dy[dir] + ny;
                                if (terrain_is_ocean(ax2, ay2) == 0)
                                    water += 2;
                            }
                            value = water >> 2;
                        } else {
                            value += g_terrain_rows[terr].value;
                        }
                        if (map_get(nx, ny) & MAP0_RIVER)
                            value++;
                        w = 2;
                        if (i < 4) {
                            w += 3;
                            if (terr == TERRAIN_OCEAN)
                                value = (value + 1) >> 1;
                        }
                        if (i < 8)
                            w += 2;
                        if (i < 0xc)
                            w++;
                        if (i < 0x14)
                            w++;
                        if (i == 0x14)
                            w = 4;
                        value = value * w >> 1;
                        total = total + value;
                    }
                }
            }
            adj = water_access(x, y);
            if (adj != 0 && continent_at(g_ocean_x, g_ocean_y) != 1)
                adj = 0;
            if (adj == 0)
                total = total >> 1;
            if (terrain_at(x, y) == TERRAIN_MOUNTAINS)
                total = 0;
            if (terrain_at(x, y) == TERRAIN_HILLS)
                total = total >> 1;
            value = minimax(total / 10, 0, 0xf);
            *site_loc(x, y) = value;
        }
        build_check(0);
    }
}
