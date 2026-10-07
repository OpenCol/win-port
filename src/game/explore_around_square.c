/* explore_around_square: candidate for 1020:89ab (347 bytes).  Verdict: RELOCATABLE.
 * symbol: explore_around_square
 *
 * Reveals the map to `nation` in a square of side `2*radius+1` centred on (x, y), and is
 * the bottom of the three-deep explore chain (`explore_square` -> `explore_area` -> here).
 * Off-map centres and nation 4 or above do nothing; nation is a 0..3 European power.
 *
 * The inner ring -- the eight neighbours, where both offsets are within 1 -- is always
 * revealed. Anything further out has to be the same *kind* of country as the centre: an
 * ocean square is revealed only to a ship and a land square only to a land unit
 * (`terrain_is_ocean(tx, ty) != is_ship` rejects it), and for a land unit the plane-2 terrain nibble
 * must equal the centre's as well, which is what stops a unit on one island from mapping
 * the next one across a strait.
 *
 * On an inner-ring square a human-controlled nation may trip a once-per-game tutorial hint: an ocean
 * square whose plane-1 bit 0x20 is set fires hint 6 and latches DGROUP:0xdca so it never
 * fires again. `g_nation_text[nation].control == CONTROL_HUMAN` gates it, so a computer or defeated nation is not told.
 *
 * Both offsets take their absolute value as `~x + 1`, which is this source's house style
 * -- `chebyshev_distance_xy`, `xy_dist` and `max_abs` all use it, and `-x` emits
 * the two-byte `NEG` instead of the shipped `NOT`/`INC`.
 *
 * The two early exits are `goto` and not `return`: the shipped bytes jump to the single
 * epilogue (rule 12), where two `return`s emit it twice.
 *
 * NAME: the Macintosh build has exactly four explore functions and their arities are
 * 1, 2, 4 and 5 -- `explore_square(short)`, `explore_area(short, short)`,
 * `explore_map_square(short, short, short, short)` and
 * `explore_around_square(short, short, short, short, short)`. This chain has the same four
 * arities in the same nesting order: `explore_square(unit)` calls `explore_area(unit,
 * radius)` calls this one with five arguments, which calls the already-matched
 * 4-argument `explore_map_square`. A bijection on arity across a whole family is
 * better evidence than any one name, and this is the member the name describes most
 * literally: it explores around a square. It is still a suggestion the call graph agrees
 * with -- a byte match proves the code, never the name.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "woodcut.h"
#include "terrain.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern int terrain_is_ocean(int x, int y);
extern unsigned char feature_get(int x, int y);
extern int land_region_at(int x, int y);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR, which says
 * it was compiled into this object. The body is matched in
 * matched/explore_map_square.c. */
extern void explore_map_square(int x, int y, int nation, int flag);

void explore_around_square(int x, int y, int nation, int radius, int is_ship)
{
    int dy;
    int ty;
    int region;
    int outer;
    register int dx;
    register int tx;

    if (on_map(x, y) == 0)
        goto done;
    if (nation >= NATION_EUROPEAN_COUNT)
        goto done;
    region = land_region_at(x, y);
    for (dy = -radius; dy <= radius; dy++) {
        for (dx = -radius; dx <= radius; dx++) {
            tx = x + dx;
            ty = y + dy;
            if (on_map(tx, ty) == 0)
                continue;
            outer = (dy > 0 ? dy : ~dy + 1) > 1 || (dx > 0 ? dx : ~dx + 1) > 1;
            if (outer) {
                if (terrain_is_ocean(tx, ty) != is_ship)
                    continue;
                if (is_ship == 0 && land_region_at(tx, ty) != region)
                    continue;
            } else if (g_hint6_shown == 0 && nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN
                       && terrain_is_ocean(tx, ty) && (feature_get(tx, ty) & MAP1_BIT5)) {
                g_hint6_shown = 1;
                woodcut(WOODCUT_DISCOVERY_OF_THE_PACIFIC_OCEAN);
            }
            explore_map_square(tx, ty, nation, outer);
        }
    }
done:
    ;
}
