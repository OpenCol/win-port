/* win_close: candidate for 1078:1d60 (40 bytes).  Verdict: RELOCATABLE.
 *
 * WM_CLOSE to the window's handle (+2) for a non-null window.
 *
 * RENAMED 2026-09-26 from win_close_1078_1d60: its 1060 namesake is now PediaWin_Done, so the suffix no longer tells two copies apart.
 */

#include "colonization.h"

#include "records.h"

void win_close(MSWin *win)
{
    if (win != 0)
        SendMessage(win->hwnd, 0x10, 0, 0L);
}
