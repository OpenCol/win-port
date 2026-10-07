/* ocean_tile_yield: candidate for 1028:b173 (89 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * A water tile's yield: 1 for a sea lane, 2 for open ocean, 5 for ocean carrying a scenery
 * decoration. Land returns 0.
 *
 * The two terrain ids are already settled elsewhere in the corpus --
 * `terrain_tile_index` maps 0x19 to the ocean tile row and 0x1a to the one after,
 * and `say_where` calls 0x1a a sea-lane square -- and `resource_at` returns
 * -1 when a tile carries no scenery sprite.
 *
 * WHICH yield is an inference and the name stops short of naming it: the function takes a
 * position and nothing else, so it produces a single number for the tile, and ocean produces
 * only one thing. 1 / 2 / 5 for sea lane / ocean / ocean-with-a-resource is the shape of the
 * food table.
 *
 * IT IS A `goto` CHAIN, not nested `if`s, and the four branches show both of rule 12's
 * forms in one function:
 *
 *     cmp di,0x19 / jz  +7        <- `if (t != 0x19) { ... }`        direct, over the block
 *     cmp di,0x1a / jz  +2 / jmp  <- `if (t != 0x1a) goto out;`     inverted, two jumps
 *     cmp di,0x1a / jnz +2 / jmp  <- `if (t == 0x1a) goto out;`     inverted, two jumps
 *     cmp ax,-1   / jnz +2 / jmp  <- `if (deco == -1) goto out;`    inverted, two jumps
 *
 * Written as `if (t == 0x19 || t == 0x1a) { ... }` the second test comes out as a single
 * direct `JNZ` and the function is four bytes shorter. And `if (t == 0x19) goto water;`
 * gives the INVERTED form for the first test, not the direct one -- rule 12 holds exactly as
 * stated, and the first line is a positive `if` whose body is one `goto`.
 */

#include "colonization.h"

#include "terrain.h"

extern int resource_at(int x, int y);

int ocean_tile_yield(int x, int y)
{
    register int n;
    register int terrain;

    n = 0;
    terrain = terrain_at(x, y);
    if (terrain != TERRAIN_OCEAN) {
        if (terrain != TERRAIN_SEA_LANE)
            goto out;
    }
    n++;
    if (terrain == TERRAIN_SEA_LANE)
        goto out;
    n++;
    if (resource_at(x, y) == -1)
        goto out;
    n += 3;
out:
    return n;
}
