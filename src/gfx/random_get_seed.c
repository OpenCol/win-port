/* random_get_seed: candidate for 1068:b6d9 (23 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 23, not the census 19. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * Reads back the seed of the game's own generator at SEG17:0x3f00, through the getter at
 * SEG17:0x01fe. The other half is `random_set_seed` (1068:b6bf) immediately above, and
 * `random_below` (1068:b6a2) is what draws from the stream. Returning the seed is how a
 * caller saves a generator position to replay it -- the only reason to expose it at all.
 *
 * See `random_set_seed` for why this is a different stream from `srand_rtl`'s.
 */

#include "colonization.h"

long random_get_seed(void)
{
    return L17_01fe();
}
