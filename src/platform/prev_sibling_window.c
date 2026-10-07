/* prev_sibling_window: candidate for 1070:9009 (71 bytes).  Verdict: RELOCATABLE.
 *
 * The previous sibling of a window (GetNextWindow 3, wrapping to GetWindow
 * 1); 0 for a handle that is not a window.
 */

#include "colonization.h"

int prev_sibling_window(int h)
{
    int w;

    if (IsWindow(h) == 0)
        return 0;
    w = GetNextWindow(h, 3);
    if (w == 0)
        w = GetWindow(h, 1);
    return w;
}

