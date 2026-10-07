/* explore_square: candidate for 1020:8b87 (182 bytes).  Verdict: RELOCATABLE.
 * symbol: explore_square
 *
 * Works out how a unit can see and reveals that much map around it -- the top of the
 * explore chain, and the one the rest of the game calls when a unit appears or arrives
 * (`create_unit`, `unit_activate`, the king's-frigate and cannot-attack
 * messages).
 *
 * The radius is 1 by default and becomes 2 three separate ways:
 *   - unit types 0xf, 0x10 and 0x11;
 *   - the owning nation holding bit 7 of its bit array (`has_father`) while the unit is
 *     NOT a ship -- outside the 0xd..0x12 band -- which is a founding father granting
 *     extended sight to land units;
 *   - and type 5 adds one on top of whatever the radius already is, so a type 5 that also
 *     qualified above ends at 3.
 * Note the third is `radius++` and not `radius = 2`: the shipped `INC DI` says so.
 *
 * The nation nibble is masked in 8 bits before promoting (rule 27).
 *
 * `has_father` arrives as TLINK's 5-byte NOP/PUSH CS/CALL NEAR -- a call to another
 * module -- while `explore_area` is the compiler's own 4-byte form, so the two lower
 * members of the chain share this object and the bit test does not.
 *
 * REGISTER ALLOCATION, and a refinement of rule 33: the shipped code holds `u` in SI and
 * `radius` in DI. Writing `register int radius` swaps them -- an explicit `register` on a
 * local OUTRANKS an enregistered parameter for SI, so the keyword changes the allocation
 * rather than merely confirming it. With neither declared `register` Borland enregisters
 * both in the shipped order, which is what this file does.
 *
 * NAME: taken from the Macintosh build, whose four explore functions have arities 1, 2, 4
 * and 5; this is the one-argument member. See matched/explore_around_square.c
 * for the family argument. A suggestion the call graph agrees with, not a proof.
 */

#include "colonization.h"

#include "fathers.h"
#include "units.h"
#include "records.h"

extern int has_father(int nation, int bit);

/* Defined above the target for the compiler's own 4-byte PUSH CS / CALL NEAR; the body is
 * matched in matched/explore_area.c. */

void explore_square(int u)
{
    int radius;

    radius = 1;
    if (g_units[u].type == UNIT_GALLEON || g_units[u].type == UNIT_PRIVATEER || g_units[u].type == UNIT_FRIGATE)
        radius = 2;
    if (has_father((unsigned char)(g_units[u].flags & UNIT_NATION_MASK), FATHER_HERNANDO_DE_SOTO)
        && (g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST))
        radius = 2;
    if (g_units[u].type == UNIT_SCOUTS)
        radius++;
    explore_area(u, radius);
}
