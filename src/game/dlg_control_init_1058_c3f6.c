/* PORT: this file's own declarations of DialogWin_AddControl removed: the header declares it from the definition (W1 step 3). */
/* dlg_control_init_1058_c3f6: candidate for 1058:c3f6 (118 bytes).  Verdict: RELOCATABLE.
 * symbol: dlg_control_init_1058_c3f6
 *
 * This module's own copy of the DlgControl initialiser: owner window, kind and id, the
 * rectangle copied in, the handle, next link and focus byte cleared, then the control registered
 * with its window. One of nine copies (two in 1060), byte-identical but for the fixups.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void dlg_control_init_1058_c3f6(DlgControl *ctl, void *win, int kind, int id, RECT *r)
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
