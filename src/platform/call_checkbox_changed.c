/* call_checkbox_changed: candidate for 1070:9952 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_checkbox_changed
 *
 * Calls the handler stored at +0x1a of the table with (a, b) when one is set --
 * the invoker paired with the set_handler_slot_26 setters.
 *
 * RENAMED 2026-09-26 from call_handler_slot_26_1070_9952: calls the checkbox's +0x1a callback with its id and the new
 * checked state after checkbox_click or checkbox_wndproc toggles it.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_checkbox_changed(SCheckBox *ctl, int id, int checked)
{
    if (ctl->on_change != 0)
        ((void (*)(int, int))ctl->on_change)(id, checked);
}
