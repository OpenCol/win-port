/* woodcut_set: candidate for 1060:046f (83 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Marks woodcut `n` shown or unshown: bit n - 1 of the bit array at SEG20:0x780c. The
 * two-argument member of the family -- see `woodcut_get` for what binds it.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

void woodcut_set(int n, int on)
{
    register int idx;
    int mask;

    n--;
    idx = n >> 3;
    mask = 1 << (n & 7);
    if (on != 0)
        g_game.woodcuts_seen[idx] |= mask;
    else
        g_game.woodcuts_seen[idx] &= ~mask;
}
