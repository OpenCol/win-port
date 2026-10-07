/* explore_area: candidate for 1020:8b06 (129 bytes).  Verdict: RELOCATABLE.
 * symbol: explore_area
 *
 * Reveals the map around a unit at a given radius: reads the unit's position, its nation
 * from the low nibble of +0x3, and whether its type is a ship (0xd..0x12), and hands all
 * five to `explore_around_square`. A pure adapter from a unit index to a map
 * square, and the middle of the explore chain.
 *
 * The nation nibble is masked in 8 bits -- `AND AL,0xf` then `MOV AH,0` -- so the source
 * casts to `unsigned char` before the argument promotes it (rule 27). Written without the
 * cast the mask happens after the promotion and the two instructions swap.
 *
 * The six unit types 0xd..0x12 are the ships; `radius` reaches the callee as the extent of
 * the square, and `explore_square` is the caller that works it out.
 *
 * NAME: taken from the Macintosh build, which has four explore functions of arity 1, 2, 4
 * and 5. This one takes two arguments, and the chain's four arities and nesting order
 * match the Macintosh set exactly -- see matched/explore_around_square.c for
 * the whole argument. A suggestion the call graph agrees with, not a proof.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR; the body is
 * matched in matched/explore_around_square.c. */

void explore_area(int u, int radius)
{
    explore_around_square(g_units[u].x, g_units[u].y,
                                    (unsigned char)(g_units[u].flags & UNIT_NATION_MASK), radius,
                                    g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST);
}
