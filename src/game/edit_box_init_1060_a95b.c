/* PORT: this file's own declarations of create_MSEditBoxClass_window, dlg_control_init_1060_aa64, win_send_settext removed: the header declares them from the definition (W1 step 3). */
/* edit_box_init_1060_a95b: candidate for 1060:a95b (97 bytes).  Verdict: RELOCATABLE.
 * symbol: edit_box_init_1060_a95b
 *
 * The edit-box initialiser: dlg control set-up with kind 4 (dlg_control_init_1060_aa64),
 * the MSEditBoxClass window created over the rectangle for the window
 * handle at +0x10, then win_send_settext with the pointer.  The set-up is
 * -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;


void edit_box_init_1060_a95b(SEditBox *edit, void *win, int id, RECT *r, void *text)
{
    dlg_control_init_1060_aa64((DlgControl *)edit, win, 4, id, r);
    edit->hwnd = create_MSEditBoxClass_window(r, edit, 1L, 1);
    win_send_settext(edit->hwnd, text);
}

