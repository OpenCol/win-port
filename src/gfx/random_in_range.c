/* random_in_range: candidate for 1068:b7e0 (60 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 60, not the census 52. The last 8 bytes are an unreachable epilogue;
 * Ghidra sizes by reachable code.
 *
 * A uniform pick from lo..hi inclusive: `lo + random_below(hi - lo + 1)`. Both the span
 * and the draw get stack slots because the draw is a call, which is what makes a
 * two-expression function 52 bytes.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int random_in_range(register int lo, int hi)
{
    int span;
    int draw;

    span = hi - lo + 1;
    draw = random_below(span);
    return lo + draw;
}
