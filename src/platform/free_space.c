/* free_space: candidate for 1070:d214 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 25. The last 8 are a second, unreachable epilogue.
 *
 * `GetFreeSpace(0)` and nothing else -- the global heap's free bytes, with flags 0.
 */

#include "colonization.h"

unsigned long free_space(void)
{
    return GetFreeSpace(0);
}
