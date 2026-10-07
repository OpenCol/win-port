/* MSSetEBLimit: candidate for 1050:8a55 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Hands the control's window handle at +0x10 and a maximum character count to `win_send_415` (EM_LIMITTEXT). Its caller is
 * `Popup_make_controls`, passing `(&p->edits[k], e->rows)`.
 *
 * RENAMED 2026-09-26 from edit_set_rows: Sends message 0x415 to an edit control's HWND via win_send_415. The control is an MSEditBoxClass window subclassed from the Windows EDIT procedure, and WINDOWS.H line 4597 defines EM_LIMITTEXT as (WM_USER+21) = 0x415. So this sets the maximum text length. The value passed (Popup_make_controls' e->rows) is a character count: the same field times the width of "W" gives the box's pixel width. The box is sized in characters, not rows, so 'set_rows' names the wrong operation.
 *
 * RENAMED 2026-09-27 from edit_limit_text: Mac MSSetEBLimit(ControlRecord**, short). It sends 0x415 (EM_LIMITTEXT) to the edit control's +0x10 window. Caller: Popup_make_controls, which is Mac MSSetEBLimit's only caller, Popup::make_controls. Arity 2 = (edit, max_chars). A singleton; win_send_415 stays a generic SendMessage helper.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void MSSetEBLimit(SEditBox *edit, int max_chars)
{
    win_send_415(edit->hwnd, max_chars);
}
