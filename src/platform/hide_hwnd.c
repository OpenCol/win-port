/* hide_hwnd: candidate for 1070:0b67 (36 bytes).  Verdict: RELOCATABLE.
 *
 * ShowWindow(h, 0) for a non-null handle.
 */

#include "colonization.h"

void hide_hwnd(int h)
{
    if (h == 0)
        return;
    ShowWindow(h, 0);
}

