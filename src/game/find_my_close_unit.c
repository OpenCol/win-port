/* find_my_close_unit: candidate for 1038:3b4e (167 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The unit of `nation` nearest to (x, y), ignoring unit `skip`; returns its index or -1,
 * and leaves the distance it settled on in DGROUP:0x4c38 so the caller can read it without
 * a second parameter. The search is a linear scan of all `g_game.unit_count` units with
 * `xy_dist` on the two offsets, starting from 9999 (0x270f), and the
 * comparison is `<=`, so among equals the LAST unit scanned wins.
 *
 * `nation` is an eight-bit parameter. The shipped compare is `CMP AL,[BP+6]` against the
 * masked nibble, with no promotion on either side; declared `int` the same source emits
 * `MOV AH,0` and a 16-bit compare (rule 27). The slot is two bytes wide either way, so the
 * call sites cannot say -- only the callee's use of it does.
 *
 * The three tests are not all spelled the same way and the bytes are why. The first two are
 * `continue`, which Borland compiles as an inverted short branch OVER an unconditional jump
 * (`JZ +2 / JMP`); the third is a positive `if` whose body is the two assignments, so it
 * gets a single `JG` straight over them. Written all three ways round the function is two
 * bytes longer and diverges at the third. This is rule 12 seen from both sides in one
 * function: what looks like an arbitrary choice of phrasing is visible in the object code.
 *
 * NAME: descriptive. The Macintosh symbol table has no `nearest`, `closest` or `find_unit`
 * of any arity, so there is nothing to adopt; the previous name, `record_nearest_of_player`,
 * said "record" where the thing being scanned is the unit array and "player" where the
 * argument is any nation, natives included.
 *
 * RENAMED 2026-09-26 from nearest_unit: the Mac build's find_my_close_unit(short, short, short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between unit_special_active and end_unit_turn, which are already bound, paired in order; the arity agrees and the Mac body is 1.02x this one.
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int xy_dist(int dx, int dy);

int find_my_close_unit(unsigned char nation, int skip, int x, int y)
{
    int best;
    int bestd;
    register int u;
    register int dist;

    best = -1;
    bestd = 0x270f;
    for (u = 0; g_game.unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != nation)
            continue;
        if (u == skip)
            continue;
        dist = xy_dist(x - g_units[u].x, y - g_units[u].y);
        if (dist <= bestd) {
            best = u;
            bestd = dist;
        }
    }
    g_nearest_unit_dist = bestd;
    return best;
}
