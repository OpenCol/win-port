/* win_set_icon_1078_2832: candidate for 1078:2832 (67 bytes).  Verdict: RELOCATABLE.
 *
 * Loads icon resource `icon` of the program (hinstance at DGROUP:0x56e2)
 * and makes it the window class's icon (SetClassWord GCW_HICON).
 */

#include "colonization.h"

#include "records.h"

void win_set_icon_1078_2832(MSWin *win, int icon)
{
    if (win == 0)
        return;
    SetClassWord(win->hwnd, -14, LoadIcon(g_hinstance, (const char *)(unsigned long)(unsigned)icon));
}
