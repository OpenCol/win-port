/* find_beach: candidate for 1028:0b21 (572 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 572, not the census 563. The trailing 9 bytes are the dead second epilogue.
 *
 * SCORES THE NINE SQUARES AROUND (x, y) -- the eight neighbours and staying put, direction 8 --
 * and returns the best direction, 8 when nothing beats it. `plan_move` is the
 * caller.
 *
 * A square is considered when it is on the map, not water, and either direction 8 or free to
 * enter: nobody on it, or the square is not a settlement, belongs to `nation`, and carries a
 * single unit whose being type 0xb does not match the `artillery` argument.
 *
 * Its score is the terrain's byte from the 16-byte row table at SEG20:0x78f0, plus, for each of
 * its own eight neighbours that is on the map and dry and holds no unit, the plane-3 nibble of
 * that neighbour plus sixteen times `region_build_factor` of the CENTRE square's land region. That
 * addition is skipped when the neighbour's plane-2 owner is a human European this nation has
 * pair flag 0x40 with -- the one case worth avoiding -- and the whole term only counts when the
 * `use_status` argument is set.
 *
 * The row base is stated as SEG20:0x78f0 with the byte at +0xb; the addresses in the image run
 * 0x78f8..0x78ff across all readers, so 0x78f0 fits them all, and the comparator cannot check it
 * either way -- the whole displacement is one fixup (RE_STATUS 250).
 *
 * `(g_units[unit].type == 0xb) != artillery` is a relational stored as a value and then compared.
 * `n` and `x2` are two `register` locals, SI and DI in declaration order (rule 53); `region_build_factor`
 * comes through the 4-byte `PUSH CS` form and is a stub here (rule 47).
 *
 * RENAMED 2026-09-26 from best_move_dir: the Mac build's find_beach(short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller plan_move; callee get_treaty; callee land_region_at; callee on_map; callee terrain_at), the runner-up gets 3, and the arity agrees (5).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "units.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern int is_anything(int x, int y);
extern int is_city(int x, int y);
extern int top_unit_in_square(int x, int y);
extern int stack_check(int u, int kind);

extern int terrain_is_ocean(int x, int y);

extern int is_unit(int x, int y);
extern char owner_of(int x, int y);
extern int get_treaty(int nation_a, int nation_b);
extern unsigned char site_get(int x, int y);
extern int land_region_at(int x, int y);

/* same object: 4-byte `push cs; call ` */
extern int region_build_factor(int nation, int region);

int find_beach(int nation, int x, int y, int use_status, int artillery)
{
    int nx;
    int ny;
    int y2;
    int score;
    int best;
    int best_dir;
    int dir;
    int d2;
    int ok;
    int unit;
    register int n;
    register int x2;

    best = -1;
    best_dir = 8;
    for (dir = 0; dir < 9; dir++) {
        nx = g_dir_dx[dir] + x;
        ny = g_dir_dy[dir] + y;
        ok = 0;
        n = is_anything(nx, ny);
        if (n < 0) {
            ok = 1;
        } else if (is_city(nx, ny) < 0 && n == nation) {
            unit = top_unit_in_square(nx, ny);
            if (stack_check(unit, 2) == 1
                && (g_units[unit].type == UNIT_ARTILLERY) != artillery)
                ok = 1;
        }
        if (on_map(nx, ny) != 0 && terrain_is_ocean(nx, ny) == 0 && (dir == 8 || ok != 0)) {
            score = 0;
            score += g_terrain_rows[terrain_at(nx, ny)].defence;
            for (d2 = 0; d2 < 8; d2++) {
                x2 = g_dir_dx[d2] + nx;
                y2 = g_dir_dy[d2] + ny;
                if (on_map(x2, y2) != 0 && terrain_is_ocean(x2, y2) == 0) {
                    n = is_unit(x2, y2);
                    if (n < 0) {
                        n = owner_of(x2, y2);
                        if ((n < 0 || n >= NATION_EUROPEAN_COUNT || g_nation_text[n].control != CONTROL_HUMAN
                             || (get_treaty(nation, n) & 0x40) == 0)
                            && use_status != 0)
                            score += (site_get(x2, y2) & 0xf)
                                   + (region_build_factor(nation, land_region_at(nx, ny)) << 4);
                    }
                }
            }
            if (score > best) {
                best = score;
                best_dir = dir;
            }
        }
    }
    return best_dir;
}
