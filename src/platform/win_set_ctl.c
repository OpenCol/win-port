/* win_set_ctl: candidate for 1070:65c6 (28 bytes).  Verdict: RELOCATABLE.
 *
 * The setter paired with `win_get_ctl` at 1070:65aa: `SetWindowLong(hwnd, 6, value)`, which
 * stores the control back-pointer in the window's extra long. The two are 28 bytes apart and
 * 0x1c bytes long each, the usual get/set pair over one slot.
 *
 * Index 6 is the control, established from the other side by `static_text_wndproc`, which opens its
 * window procedure with `ctl = GetWindowLong(w, 6);`.
 */

#include "colonization.h"

#include "w16types.h"

void win_set_ctl(unsigned hwnd, long ctl)
{
    SetWindowLong(hwnd, 6, ctl);
}
