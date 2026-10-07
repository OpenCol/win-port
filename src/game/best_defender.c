/* best_defender: candidate for 1020:1f8c (445 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 445, not the census 436. The trailing 9 bytes are the dead second epilogue.
 *
 * WHICH UNIT OF A STACK DEFENDS AGAINST `attacker`, or -1. Every unit in the chain scores
 * `get_defense(unit, attacker) << 8` plus `255 - get_combat(unit, 0)`, and the
 * highest score wins ties by being LAST -- the test is `>=`, and the scores are unsigned, which
 * is what makes the shift below a `SHR` and the comparison a `JC`.
 *
 * Type 0xb is adjusted twice over: on a square with no European owner it is worth an eighth
 * unless its orders are 5 or 6, and on one with an owner it is worth double against a native
 * attacker.
 *
 * Two things disqualify a unit outright: on a European square, a type that cannot attack (+6 of
 * the type record is zero), and, once the square is known, being a ship where the square is land
 * or the other way round -- `(type in 0xd..0x12) != terrain_is_ocean(x, y)`.
 *
 * The square is read from the FIRST unit of the stack, before the walk starts from the tail;
 * `have` records that `on_map` agreed, and when it did not, the land/water test is skipped and
 * `sea` is never written.
 *
 * `get_defense` and `get_combat` come through the 5-byte NOP form and are extern
 * (rule 47); `unit` and `score` are two `register` locals, SI and DI in declaration order.
 *
 * A LIKELY SHIPPED SLIP AT THE CALL SITES: `attacker` is used as a UNIT index here
 * (get_defense(unit, attacker), g_units[attacker].flags), as get_defense uses its own second
 * argument, but both callers pass a NATION -- combat_fight's `anation` (the attacking unit's
 * flags & 0xf) and indian_move's `nation` -- so units 0..11 stand in for the attacker. The
 * bytes fix both sides; passing the attacking unit reads like what was meant.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern int is_colony(int x, int y);

extern int terrain_is_ocean(int x, int y);
extern int top_unit(int u);
extern int next_unit(int u);
extern int get_defense(int u, int attacker);
extern int get_combat(int u, int attacking);

int best_defender(int u, int attacker)
{
    int best;
    int sea;
    int euro;
    int x;
    int y;
    int have;
    unsigned best_score;
    register int unit;
    register unsigned score;

    best = -1;
    have = 0;
    best_score = 0;
    if (u >= 0) {
        unit = u;
        x = g_units[unit].x;
        y = g_units[unit].y;
        euro = is_colony(x, y) >= 0;
        if (on_map(x, y) != 0) {
            sea = terrain_is_ocean(x, y);
            have = 1;
        }
    }
    for (unit = top_unit(u); unit >= 0; unit = next_unit(unit)) {
        score = (get_defense(unit, attacker) << 8)
              + (0xff - get_combat(unit, 0));
        if (g_units[unit].type == UNIT_ARTILLERY) {
            if (euro == 0) {
                if (g_units[unit].orders != ORDERS_FORTIFY && g_units[unit].orders != ORDERS_FORTIFIED)
                    score = score >> 3;
            } else if ((unsigned char)(g_units[attacker].flags & UNIT_NATION_MASK) >= 4) {
                score = score * 2;
            }
        }
        if (euro != 0 && g_unit_types[g_units[unit].type].attack == 0)
            goto next;
        if (have != 0
            && (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) != sea)
            goto next;
        if (score >= best_score) {
            best_score = score;
            best = unit;
        }
next:
        ;
    }
    return best;
}
