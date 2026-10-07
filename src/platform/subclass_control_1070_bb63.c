/* subclass_control_1070_bb63: candidate for 1070:bb63 (52 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the long in window word 0x10 of the control's window (+2) and
 * makes port_paint_wndproc its window procedure (GWL_WNDPROC, -4).
 *
 * Its `obj` is a pointer, as every caller passes one (2026-10-02; it was a `long`, the same pushes).
 */

#include "colonization.h"

#include "records.h"

void subclass_control_1070_bb63(void *obj, MSWin *control)
{
    SetWindowLong(control->hwnd, 0x10, (long)obj);
    SetWindowLong(control->hwnd, -4, (long)(void *)port_paint_wndproc);
}

