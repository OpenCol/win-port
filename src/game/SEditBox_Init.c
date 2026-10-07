/* SEditBox_Init: candidate for 1060:9015 (97 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: SEditBox_Init
 *
 * Builds an edit box w wide and 30 high at (x, y): the long at +0x1a
 * cleared, the rectangle formed, edit_box_init_1060_a95b does the rest (-called,
 * defined in-file as a stub).
 *
 * RENAMED 2026-09-27 from edit_box_create_1060_9015: Mac SEditBox::Init(DialogWin&, short, short, short, short, char*), 184 bytes, against 97 (+97 in its helper). Caller: Popup::make_controls, 1 of 1 on both. Width: this+6 = 7 = (edit, win, id, x, y, w, text). Callees: Mac AddControl, MSInitEditBox, MSSetEBContents; Windows edit_box_init_1060_a95b does the rest (create_MSEditBoxClass_window, dlg_control_init, win_send_settext), the same three steps one level down. MEDIUM because layout cannot confirm: Mac SEditBox::Init opens CODE 3, and this sits among 1060's per-module UI helpers. A 0-caller twin with a style argument, edit_box_create, has no Mac counterpart and keeps its name.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern void edit_box_init_1060_a95b(SEditBox *edit, void *win, int id, RECT *r, void *text);

void SEditBox_Init(SEditBox *edit, DialogWin *win, int id, int x, int y, int w, void *text)
{
    RECT r;

    edit->on_change = 0;
    SetRect(&r, x, y, x + w, y + 0x1e);
    edit_box_init_1060_a95b(edit, win, id, &r, text);
}
