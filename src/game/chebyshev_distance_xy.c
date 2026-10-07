/* chebyshev_distance_xy: candidate for 1040:7c3e (102 bytes).  Verdict: RELOCATABLE.
 * symbol: chebyshev_distance_xy
 *
 * The Chebyshev distance of the delta between (x1, y1) and (x2, y2).  The
 * distance helper is -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

int chebyshev_distance_xy(int x1, int y1, int x2, int y2)
{
    int dx;
    int dy;

    dx = x1 - x2 > 0 ? x1 - x2 : ~(x1 - x2) + 1;
    dy = y1 - y2 > 0 ? y1 - y2 : ~(y1 - y2) + 1;
    return max_abs(dx, dy);
}

