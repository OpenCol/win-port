/* PORT: this file's own declarations of DialogWin_AddControl removed: the header declares it from the definition (W1 step 3). */
/* dlg_control_init_1068_e478: candidate for 1068:e478 (118 bytes).  Verdict: RELOCATABLE.
 * symbol: dlg_control_init_1068_e478
 *
 * This module's own copy of the DlgControl initialiser: owner window, position, the
 * rectangle copied in, the handle, long and byte cleared, then the control registered
 * with its window. One of eight per-module copies, byte-identical but for the fixups.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void dlg_control_init_1068_e478(DlgControl *ctl, void *win, int kind, int id, RECT *r)
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
