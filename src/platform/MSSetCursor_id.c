/* MSSetCursor_id: candidate for 1078:29b6 (122 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Loads the window's cursor: -1 is the arrow and -2 the hourglass (IDC_WAIT) from the system, and any other value is a cursor resource of this instance.
 *
 * RENAMED 2026-09-27 from win_load_cursor_1078_29b6: the Mac build's two-argument MSSetCursor(GrafPort*, short) overload: it records cursor `id` on the window (the stock arrow/wait for -1/-2, else a resource); the three-argument overload, MSSetCursor, calls it, so this copy carries the _id suffix.
 */

#include "colonization.h"

#include "records.h"

void MSSetCursor_id(MSWin *win, int id)
{
    if (win == 0)
        return;
    if (id < 0) {
        if (id == -1)
            win->hcursor = LoadCursor(0, (const char *)0x7f00L);
        else if (id == -2)
            win->hcursor = LoadCursor(0, (const char *)0x7f02L);
    } else {
        win->hcursor = LoadCursor(g_hinstance, (const char *)(unsigned long)(unsigned)id);
    }
}
