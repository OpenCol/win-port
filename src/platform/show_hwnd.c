/* show_hwnd: candidate for 1070:0b43 (36 bytes).  Verdict: RELOCATABLE.
 *
 * ShowWindow(h, 5) for a non-null handle.
 */

#include "colonization.h"

void show_hwnd(int h)
{
    if (h == 0)
        return;
    ShowWindow(h, 5);
}

