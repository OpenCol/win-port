/* call_radio_rbutton_down: candidate for 1070:9a34 (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_radio_rbutton_down
 *
 * Calls the handler stored at +0x22 of the table with (a, b) when one is set --
 * the invoker paired with the set_handler_slot_34 setters.
 *
 * RENAMED 2026-09-26 from call_handler_slot_34: calls the radio set's +0x22 callback with its id and the item on
 * WM_RBUTTONDOWN, taking the capture.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_radio_rbutton_down(SRadioSet *ctl, int id, int index)
{
    if (ctl->on_rbutton_down != 0)
        (*(void (*)(int, int))ctl->on_rbutton_down)(id, index);
}
