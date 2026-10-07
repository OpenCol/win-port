/* call_image_button_click: candidate for 1070:98d5 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_image_button_click
 *
 * Invokes the callback at +26 (0x1a) of the image button record it is given with one word
 * argument when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_26_1070_98d5: calls the image button's +0x1a callback with its id when a press
 * is released inside it or Space presses it (image_button_proc).
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_image_button_click(SImageButton *ctl, int id)
{
    if (ctl->on_click != 0)
        ((void (*)(int))ctl->on_click)(id);
}
