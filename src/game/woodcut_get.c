/* woodcut_get: candidate for 1060:04c2 (70 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Has woodcut `n` been shown? Bit n - 1 of the bit array at SEG20:0x780c.
 *
 * The name is the Macintosh build's, and it is the ARITY of a family that settles it rather
 * than this function alone: `core` declares `woodcut(short)`, `woodcut_get(short)`,
 * `woodcut_set(short, short)` and `woodcut_show(short)`, and segment 1060 holds four
 * functions -- 1060:003a, 04c2, 046f and 0508 -- with exactly those arities, adjacent, over
 * one bit array. `woodcut_show` at 1060:003a was already bound from the TEXT identifier WOODCUT,
 * so the family is known to live here; the other three follow.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

int woodcut_get(int n)
{
    int idx;
    int mask;

    n--;
    idx = n >> 3;
    mask = 1 << (n & 7);
    return (unsigned char)(g_game.woodcuts_seen[idx] & mask);
}
