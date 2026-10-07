/* DialogWin_AddControl: candidate for 1060:9980 (92 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Appends a control to the window's control list: the head at +0x8c when
 * empty, otherwise the next pointer (+0x12) of the last control.
 *
 * RENAMED 2026-09-26 from register_control: the Mac build's DialogWin::AddControl(void*): this plus a control, appended to the dialog's control list at +0x8c. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void DialogWin_AddControl(DialogWin *win, DlgControl *ctl)
{
    DlgControl *p;

    p = win->first_ctl;
    if (p == 0) {
        win->first_ctl = ctl;
    } else {
        while (p->next != 0)
            p = p->next;
        p->next = ctl;
    }
}
