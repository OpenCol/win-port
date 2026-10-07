/* edit_box_init_1060_a9bc: candidate for 1060:a9bc (98 bytes).  Verdict: RELOCATABLE.
 * symbol: edit_box_init_1060_a9bc
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
extern int create_MSEditBoxClass_window(RECT *r, void *edit, long l, int visible);
extern void win_send_settext(int h, void *text);

extern void dlg_control_init_1060_aa64(void *ctl, void *win, int kind, int id, RECT *r);

void edit_box_init_1060_a9bc(SEditBox *edit, void *win, int id, RECT *r, void *text, int style)
{
    dlg_control_init_1060_aa64((DlgControl *)edit, win, 4, id, r);
    edit->hwnd = create_MSEditBoxClass_window(r, edit, 1L, style);
    win_send_settext(edit->hwnd, text);
}

