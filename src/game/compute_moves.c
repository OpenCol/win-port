/* compute_moves: candidate for 1038:c7ad (729 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Builds the coarse grid `find_4` reads: the map in 4x4 blocks, 15 across by 18
 * down, with one byte table per kind -- SEG27:0xae for land (kind 0), SEG27:0x1bc for water (kind 1).
 * Each block's usable square is the one `get_valid_square_2` finds in the 2x2 at its
 * centre. For each of the first four directions in the direction table, the neighbouring block's
 * square is found the same way, and when both finds answer the same plane-2 value and
 * `move_dist` gives a path between them of 1 to 7 steps, bit d is set in this
 * block's byte and bit (d + 4) & 7 -- the opposite direction -- in the neighbour's, when the
 * neighbour lies inside the grid. The word at DGROUP:0x10a2 is 1 for the duration, and the timer
 * tick is checked after each column of blocks.
 *
 * Then a census of the 16 plane-2 values: for every square on the map, from the last column back,
 * whose value is not negative, the word array at SEG27:0x8c counts the squares of that value and
 * the byte array at SEG20:0xda50 counts those whose terrain is below 0x18 with low three bits 2 to 5.
 * The row loop runs to the map height inclusive.
 *
 * THE TWO COARSE GRIDS, named here because this function is what makes the pair legible.
 * Pass 0 fills `g_coarse_grid_land` (SEG27:0xae) with `kind = 0` and pass 1 fills
 * `g_coarse_grid_sea` (SEG27:0x1bc) with `kind = 1`, each 15 by 18 bytes. The addresses
 * confirm the size: 0xae + 0x10e is 0x1bc, so the two are adjacent and exactly 0x10e apart,
 * which is the `memset_rtl(grid, 0, 0x10e)` above. `far_move` picks between them
 * with `ship ? g_coarse_grid_sea[cx][cy] : g_coarse_grid_land[cx][cy]`.
 *
 * RENAMED 2026-09-26 from build_coarse_grid: the Mac build's compute_moves(void), bound by call-graph agreement (4 vs 2): it builds the 4x4 coarse grid find_4 reads when the AI plans moves; the arity agrees and the Mac body is 0.96x this one.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

extern int move_dist(int tx, int ty, int x, int y, int want, int limit);
extern int build_check(int flag);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_valid_square_2(int x0, int y0, int *px, int *py, int kind);

extern int land_region_at(int x, int y);

void compute_moves(void)
{
    int i;
    int j;
    int x;
    int y;
    int tx;
    int ty;
    int nx2;
    int ny2;
    int found;
    int v2;
    int dir;
    int pass;
    int kind;
    int steps;
    int terrain;
    int n;
    unsigned char *grid;
    register int nx;
    register int ny;

    g_flat_step_cost = 1;
    for (pass = 0; pass < 2; pass++) {
        if (pass != 0) {
            grid = (unsigned char *)g_coarse_grid_sea;
            kind = 1;
        } else {
            grid = (unsigned char *)g_coarse_grid_land;
            kind = 0;
        }
        memset_rtl(grid, 0, 0x10e);
        for (i = 0; i < 15; i++) {
            for (j = 0; j < 18; j++) {
                x = i * 4 + 1;
                y = j * 4 + 1;
                found = get_valid_square_2(x, y, &tx, &ty, kind);
                if (found >= 0) {
                    for (dir = 0; dir < 4; dir++) {
                        nx = g_dir_dx[dir] * 4 + x;
                        ny = g_dir_dy[dir] * 4 + y;
                        if (on_map(nx, ny) == 0)
                            continue;
                        v2 = get_valid_square_2(nx, ny, &nx2, &ny2, kind);
                        if (v2 == found) {
                            steps = move_dist(tx, ty, nx2, ny2, kind, 9);
                            if (steps > 0 && steps < 8) {
                                grid[i * 18 + j] |= 1 << dir;
                                nx = g_dir_dx[dir] + i;
                                ny = g_dir_dy[dir] + j;
                                if (nx >= 0 && ny >= 0 && nx < 15 && ny < 18)
                                    grid[nx * 18 + ny] |= 1 << ((dir + 4) & 7);
                            }
                        }
                    }
                }
            }
            build_check(0);
        }
    }
    g_flat_step_cost = 0;
    for (n = 0; n < 16; n++) {
        g_region_size[n] = 0;
        g_region_farmland[n] = 0;
    }
    for (y = 0; y <= g_map_size.h; y++) {
        for (x = g_map_size.w - 1; x >= 0; x--) {
            if (on_map(x, y)) {
                terrain = terrain_at(x, y);
                n = land_region_at(x, y);
                if (n >= 0) {
                    if (terrain < 0x18) {
                        terrain = terrain & 7;
                        if (terrain >= 2 && terrain <= 5)
                            g_region_farmland[n]++;
                    }
                    g_region_size[n]++;
                }
            }
        }
        build_check(0);
    }
}
