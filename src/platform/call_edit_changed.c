/* call_edit_changed: candidate for 1070:a0b7 (42 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_edit_changed
 *
 * Calls the handler stored at +0x1a of the table with the table's own word at +2
 * when one is set.
 *
 * RENAMED 2026-09-26 from call_handler_slot_26_self: calls an edit control's +0x1a callback with the control's own id;
 * dialog_wndproc fires it on the EN_CHANGE notification.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_edit_changed(SEditBox *edit)
{
    if (edit->on_change != 0)
        (*(void (*)(int))edit->on_change)(edit->id);
}
