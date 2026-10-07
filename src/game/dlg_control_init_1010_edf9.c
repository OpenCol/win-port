/* PORT: this file's own declarations of DialogWin_AddControl removed: the header declares it from the definition (W1 step 3). */
/* dlg_control_init_1010_edf9: candidate for 1010:edf9 (118 bytes).  Verdict: RELOCATABLE.
 *
 * The colony module's DlgControl initialiser: owner window, position, the
 * rectangle copied in (F_SCOPY), the handle, long and byte cleared, then
 * DialogWin_AddControl registers the control with its window.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void dlg_control_init_1010_edf9(DlgControl *ctl, void *win, int kind, int id, RECT *r)
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
