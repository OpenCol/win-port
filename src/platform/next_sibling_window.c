/* next_sibling_window: candidate for 1070:8fc2 (71 bytes).  Verdict: RELOCATABLE.
 *
 * The next sibling of a window (GetNextWindow 2, wrapping to GetWindow
 * 0); 0 for a handle that is not a window.
 */

#include "colonization.h"

int next_sibling_window(int h)
{
    int w;

    if (IsWindow(h) == 0)
        return 0;
    w = GetNextWindow(h, 2);
    if (w == 0)
        w = GetWindow(h, 0);
    return w;
}

