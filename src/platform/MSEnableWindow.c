/* MSEnableWindow: candidate for 1078:1d00 (36 bytes).  Verdict: RELOCATABLE.
 *
 * EnableWindow on the window's handle (+2) for a non-null window.
 */

#include "colonization.h"

#include "records.h"

void MSEnableWindow(MSWin *win, int on)
{
    if (win != 0)
        EnableWindow(win->hwnd, on);
}
