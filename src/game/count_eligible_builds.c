/* count_eligible_builds: candidate for 1018:8baf (52 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 52, not the census 46. The trailing 6 bytes are the dead second epilogue.
 *
 * How many construction items `eligible_to_build` accepts over -1..0x30 -- the count to
 * `nth_eligible_build`'s index, and it sits fifty bytes above it. The -1 is a real
 * item, not a sentinel: the loop starts there.
 *
 * RENAMED 2026-09-26 from count_buildable: the Mac build's count_eligible_builds(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between eligible_to_build and compute_colony_dynamic, where the Mac gap holds 2 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.42x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` */
extern int eligible_to_build(int item);

int count_eligible_builds(void)
{
    register int n;
    register int i;

    n = 0;
    for (i = -1; i < 0x31; i++) {
        if (eligible_to_build(i) != 0)
            n++;
    }
    return n;
}
