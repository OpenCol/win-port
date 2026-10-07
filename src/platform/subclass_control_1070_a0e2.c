/* subclass_control_1070_a0e2: candidate for 1070:a0e2 (52 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the long in window word 0xc of the control's window (+2) and
 * makes port_wndproc its window procedure (GWL_WNDPROC, -4).
 *
 * Its `win` is a pointer, as every caller passes one (2026-10-02; it was a `long`, the same pushes).
 */

#include "colonization.h"

#include "records.h"

void subclass_control_1070_a0e2(void *win, MSWin *control)
{
    SetWindowLong(control->hwnd, 0xc, (long)win);
    SetWindowLong(control->hwnd, -4, (long)(void *)port_wndproc);
}

