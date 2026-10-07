/* call_radio_changed: candidate for 1070:99e2 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_radio_changed
 *
 * Calls the handler stored at +0x1a of the table with (a, b) when one is set --
 * the invoker paired with the set_handler_slot_26 setters.
 *
 * RENAMED 2026-09-26 from call_handler_slot_26_1070_99e2: calls the radio set's +0x1a callback with its id and the current
 * item after a click (radio_click) or an arrow key (radio_wndproc) changes the selection.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_radio_changed(SRadioSet *ctl, int id, int item)
{
    if (ctl->on_change != 0)
        (*(void (*)(int, int))ctl->on_change)(id, item);
}
