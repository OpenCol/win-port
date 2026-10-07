/* invalidate_hwnd: candidate for 1070:0b1c (39 bytes).  Verdict: RELOCATABLE.
 *
 * InvalidateRect(h, NULL, FALSE) for a non-null handle.
 */

#include "colonization.h"

#include "w16types.h"

void invalidate_hwnd(int h)
{
    if (h == 0)
        return;
    InvalidateRect(h, 0, 0);
}
