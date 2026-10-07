/* MSSetBackground: candidate for 1078:1ed6 (81 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Replaces the window's background brush with a solid one in the given palette entry and invalidates the window; PALETTEINDEX is what the 0x01000000 bit says.
 *
 * RENAMED 2026-09-27 from win_set_back_color_1078_1ed6: the Mac build's MSSetBackground(GrafPort*, char): the window-level routine -- a solid brush from palette index `index`, then an invalidate; its object-level forwarders keep their descriptive names, as MSSetCursor_of does.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

void MSSetBackground(MSWin *win, char index)
{
    if (win->hbrush != 0)
        DeleteObject(win->hbrush);
    win->hbrush = CreateSolidBrush(0x01000000L | (unsigned long)(unsigned)index);
    InvalidateRect(win->hwnd, 0, 0);
}
