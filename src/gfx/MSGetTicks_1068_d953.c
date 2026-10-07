/* MSGetTicks_1068_d953: candidate for 1068:d953 (23 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 23, not the census 19. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * This module's -callable forwarder to `MSGetTicks` in segment 1078, named for the
 * import it reaches, as the rest of the wrapper layer is. Three call sites in segment
 * 1068 use it, `random_seed_from_ticks` (1068:b6f0) among them; a call costs four
 * bytes where the call it wraps costs five plus a fixup, which is the whole point of
 * a per-module forwarder.
 */

#include "colonization.h"

long MSGetTicks_1068_d953(void)
{
    return MSGetTicks();
}
