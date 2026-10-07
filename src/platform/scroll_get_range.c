/* scroll_get_range: candidate for 1070:5fbf (36 bytes).  Verdict: RELOCATABLE.
 *
 * GetScrollRange on the window's SB_CTL bar (2).
 */

#include "colonization.h"

void scroll_get_range(int h, int *lo, int *hi)
{
    GetScrollRange(h, 2, lo, hi);
}
