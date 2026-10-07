/* purchase_available: candidate for 1028:33e9 (25 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Returns 1 through a local the source clears first, which is why the dead
 * `XOR DX,DX` survives at -Od.
 *
 * It takes an argument it never reads. Its one caller, `try_to_purchase`, pushes the
 * recruit kind and cleans two bytes -- and an unused parameter is invisible in the callee's
 * own 25 bytes (§119). Whatever this was meant to decide, it now says yes to everything.
 *
 * RENAMED 2026-09-26 from return_one_1028_33e9: the Mac build's purchase_available(short), bound by layout (between foreign_unit and try_to_purchase on both sides); the arity agrees and the Mac body is 1.52x this one. The Windows body only returns 1.
 */

#include "colonization.h"

int purchase_available(int kind)
{
    register int r = 0;

    r = 1;
    return r;
}
