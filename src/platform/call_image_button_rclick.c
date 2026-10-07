/* call_image_button_rclick: candidate for 1070:98fb (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_image_button_rclick
 *
 * Invokes handler slot +30 (0x1e) of the widget layer's handler table with one word
 * argument when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_30_1070_98fb: calls the image button's +0x1e callback with its id when a
 * right-button press is released inside it (image_button_proc, WM_RBUTTONUP).
 * The body is the generic null-checked call; the one control class that calls it names it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_image_button_rclick(SImageButton *ctl, int id)
{
    if (ctl->on_rclick != 0)
        ((void (*)(int))ctl->on_rclick)(id);
}
