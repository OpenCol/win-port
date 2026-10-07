/* scroll_set_range: candidate for 1070:5f8f (48 bytes).  Verdict: RELOCATABLE.
 *
 * SetScrollRange on the window's SB_CTL bar (2) with redraw, then the
 * position set to the low end.
 */

#include "colonization.h"

void scroll_set_range(int h, int lo, int hi)
{
    SetScrollRange(h, 2, lo, hi, 1);
    scroll_set_pos(h, lo);
}
