/* win_get_ctl: candidate for 1070:65aa (24 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the control a window carries in its extra long at index 6 -- `GetWindowLong(hwnd, 6)`
 * and nothing else. The whole body is the -ml -WS prologue, two pushes and the call, so the
 * API's own return in DX:AX is this function's.
 *
 * INDEX 6 IS THE CONTROL, which the corpus establishes from the other side: `static_text_wndproc` opens
 * its window procedure with `ctl = GetWindowLong(w, 6);` and then treats it as the control it
 * is painting. It is not one of the negative `GWL_*` constants -- a positive index reads the
 * class's own extra bytes, which is where this program keeps the back-pointer.
 */

#include "colonization.h"

#include "w16types.h"

long win_get_ctl(unsigned hwnd)
{
    return GetWindowLong(hwnd, 6);
}
