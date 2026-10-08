/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* subclass_control_1070_8f26: candidate for 1070:8f26 (52 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the long in window word 8 of the control's window (+2) and
 * makes dialog_wndproc its window procedure (GWL_WNDPROC, -4).
 *
 * Its `control` is a pointer, as every caller passes one (2026-10-02; it was a `long`, the same pushes).
 */

#include "colonization.h"

#include "records.h"

void subclass_control_1070_8f26(void *control, MSWin *win)
{
    SetWindowLong(win->hwnd, 8, (intptr_t)control);
    SetWindowLong(win->hwnd, -4, (intptr_t)(void *)dialog_wndproc);
}

