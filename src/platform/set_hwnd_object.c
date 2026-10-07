/* set_hwnd_object: candidate for 1070:67bd (32 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the object pointer in window word 0: SetWindowLong(w->hwnd, 0, obj). This is the
 * back-pointer every window procedure in the platform layer reads to find its object.
 */

#include "colonization.h"

#include "records.h"

void set_hwnd_object(void *obj, MSWin *win)
{
    SetWindowLong(win->hwnd, 0, (long)obj);
}
