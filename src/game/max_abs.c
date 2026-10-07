/* max_abs: candidate for 1040:7bfc (66 bytes).  Verdict: EXACT.
 *
 * The larger of |a| and |b| (the Chebyshev distance of a delta).
 */

#include "colonization.h"

int max_abs(int dx, int dy)
{
    int r;

    dx = dx > 0 ? dx : ~dx + 1;
    dy = dy > 0 ? dy : ~dy + 1;
    if (dx > dy)
        r = dx;
    else
        r = dy;
    return r;
}
