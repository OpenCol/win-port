/* controls_invalidate: candidate for 1060:9afa (69 bytes).  Verdict: RELOCATABLE.
 *
 * Invalidates the window of every control on the window's control list
 * (head at +0x8c, next pointers at +0x12 of each control).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void controls_invalidate(DialogWin *w)
{
    DlgControl *ctl;

    for (ctl = w->first_ctl; ctl != 0; ctl = ctl->next)
        invalidate_hwnd(ctl->hwnd);
}
