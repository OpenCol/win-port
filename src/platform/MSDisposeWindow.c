/* PORT: this file's own declarations of remove_ms_window removed: the header declares it from the definition (W1 step 3). */
/* MSDisposeWindow: candidate for 1078:1d88 (164 bytes).  Verdict: RELOCATABLE.
 *
 * This tears a platform WINDOW record down (the census called it menu_attach_to_window and the
 * corpus followed, reading the SetMenu as the point): its window (+2) is hidden,
 * detached and destroyed unless the word at +0x22 says it was already destroyed (main_window_message_handler sets it on WM_DESTROY), the GDI object at +8
 * deleted, and its own handle (+0) unlocked and disposed. Always returns 0 -- Screen's
 * destructor assigns the result back to its window field.
 *
 * RENAMED 2026-09-26 from menu_attach_to_window: it frees a platform window record, not a menu: the record's window (+2) is hidden, its menu
 * detached (SetMenu 0) and the window destroyed unless the window was already destroyed (+0x22, set on WM_DESTROY), the GDI object at
 * +8 deleted, and the record's own handle unlocked and disposed; it returns 0 so the caller clears its
 * pointer. Named like its siblings MSDisposeButton, MSDisposeCheck, MSDisposeRadio, MSDisposeRegion.
 */

#include "colonization.h"

#include "records.h"

void *MSDisposeWindow(MSWin *win)
{
    unsigned h;

    if (win != 0) {
        remove_ms_window(win->hwnd);
        if (win->destroyed == 0) {
            if (win->hwnd != 0) {
                ShowWindow(win->hwnd, 0);
                SetMenu(win->hwnd, 0);
                DestroyWindow(win->hwnd);
            }
            win->hwnd = 0;
        }
        if (win->hbrush != 0)
            DeleteObject(win->hbrush);
        h = win->handle;
        MSHUnlock(h);
        MSDisposeHandle(h);
    }
    return 0;
}
