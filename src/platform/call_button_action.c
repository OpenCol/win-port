/* call_button_action: candidate for 1070:9863 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_button_action
 *
 * Invokes handler slot +28 (0x1c) of the widget layer's handler table with one word
 * argument when a handler is installed.
 *
 * Not the object table's double-click slot, which `call_on_ldblclk` invokes with TWO
 * arguments (x, y): same offset, a different table. It was call_handler_slot_28_1070_9863;
 * named for the one argument that tells the two apart.
 *
 * RENAMED 2026-09-26 from call_handler_slot_28_1arg: calls the button's +0x1c callback -- the one set_button_action
 * stores -- with its id, from MSPushButton and bitmap_button_proc.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_button_action(SButton *ctl, int id)
{
    if (ctl->action != 0)
        ((void (*)(int))ctl->action)(id);
}
