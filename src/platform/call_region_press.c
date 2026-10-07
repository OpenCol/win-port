/* call_region_press: candidate for 1070:9745 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_region_press
 *
 * Invokes handler slot +38 (0x26) of the widget layer's handler table with one word
 * argument when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_38: calls the region's +0x26 callback on WM_LBUTTONDOWN while
 * the pointer is over it.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 *
 * RENAMED 2026-09-26 from call_hover_press: renamed with its procedure; the region control's: MSInitRegion (a Mac-build name) subclasses every
 * region window with region_wndproc, and SRegion's five callbacks at +0x1a..+0x2a are the five this
 * procedure fires. It was named for its behaviour before its class was known.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_region_press(SRegion *ctl, int id)
{
    if (ctl->on_press != 0)
        ((void (*)(int))ctl->on_press)(id);
}
