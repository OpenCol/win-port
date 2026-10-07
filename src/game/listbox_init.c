/* PORT: this file's own declarations of create_MSListBoxClass_window removed: the header declares it from the definition (W1 step 3). */
/* listbox_init: candidate for 1050:865a (82 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Initialises a list-box control: the ordinary `dlg_control_init_1050_9418` with the control kind fixed
 * at 7, then `create_MSListBoxClass_window` to make the window, its HWND into the control's +0x10,
 * and the derived word at +0x1a cleared.
 *
 * The kind of 7 is a literal in the call, not a parameter -- it is the list-box control
 * kind, as 6 is the button's.
 *
 * +0x1a is past the end of `DlgControl` (0x1a bytes), which is how the object is known to be
 * a derived class; `SListBox_ctor` 78 bytes above (1050:860c) allocates 0x20 for it and zeroes
 * the long at +0x1c, and `SListBox_dtor` at 1050:8229 was in the corpus before either. (Until
 * 2026-09-28 the constructor names at 860c and 86d6 were swapped; the 0x22-byte class 124 bytes
 * below is SCheckBox's.)
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern void dlg_control_init_1050_9418(SListBox *ctl, void *win, int kind, int id, RECT *r);

void listbox_init(SListBox *listbox, DialogWin *win, int id, RECT *r)
{
    dlg_control_init_1050_9418(listbox, win, 7, id, r);
    listbox->hwnd = create_MSListBoxClass_window(r, listbox, 1);
    listbox->count = 0;
}
