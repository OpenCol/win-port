/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* subclass_control_1070_67dd: candidate for 1070:67dd (52 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the long in window word 4 of the control's window (+2) and
 * makes object_message_handler its window procedure (GWL_WNDPROC, -4).
 *
 * Its `obj` is a pointer, as every caller passes one (2026-10-02; it was a `long`, the same pushes).
 */

#include "colonization.h"

#include "records.h"

void subclass_control_1070_67dd(void *obj, MSWin *control)
{
    SetWindowLong(control->hwnd, 4, (intptr_t)obj);
    SetWindowLong(control->hwnd, -4, (intptr_t)(void *)object_message_handler);
}

