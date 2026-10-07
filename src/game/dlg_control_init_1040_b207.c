/* PORT: this file's own declarations of DialogWin_AddControl removed: the header declares it from the definition (W1 step 3). */
/* dlg_control_init_1040_b207: candidate for 1040:b207 (118 bytes).  Verdict: RELOCATABLE.
 *
 * Segment 1040's copy of the DlgControl initialiser: owner window,
 * position, the rectangle copied in, handle, long and byte cleared, then
 * DialogWin_AddControl registers the control with its window.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void dlg_control_init_1040_b207(DlgControl *ctl, void *win, int kind, int id, RECT *r)
{
    ctl->win = win;
    ctl->kind = kind;
    ctl->id = id;
    ctl->rect = *r;
    ctl->next = 0;
    ctl->hwnd = 0;
    ctl->initial_focus = 0;
    DialogWin_AddControl((DialogWin *)win, ctl);
}
