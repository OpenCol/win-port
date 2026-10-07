/* call_radio_dblclk: candidate for 1070:9a0b (41 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_radio_dblclk
 *
 * Calls the handler stored at +0x1e of the table with (a, b) when one is set --
 * the invoker paired with the plain-store set_radio_dblclk_1068_e056 / _1050_88d6 setters.
 *
 * RENAMED 2026-09-26 from call_handler_slot_30: calls the radio set's +0x1e callback with its id and the item on
 * WM_LBUTTONDBLCLK, after selecting that item.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_radio_dblclk(SRadioSet *ctl, int id, int index)
{
    if (ctl->on_dblclk != 0)
        (*(void (*)(int, int))ctl->on_dblclk)(id, index);
}
