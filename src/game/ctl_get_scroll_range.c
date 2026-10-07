/* ctl_get_scroll_range: candidate for 1060:aa1e (41 bytes).  Verdict: RELOCATABLE.
 *
 * scroll_get_range on the control's window handle (+0x10), the two out-pointers passed through.
 *
 * RENAMED 2026-09-26 from control_call_5fbf: scroll_get_range on the control's window handle (+0x10),
 * the two out-pointers passed through.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ctl_get_scroll_range(DlgControl *control, int *lo, int *hi)
{
    scroll_get_range(control->hwnd, lo, hi);
}
