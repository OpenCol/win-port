/* call_region_click: candidate for 1070:976b (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_region_click
 *
 * Invokes handler slot +34 (0x22) of the widget layer's handler table with one word
 * argument when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_34_1070_976b: calls the region's +0x22 callback when a press is released
 * with the pointer still over the button.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 *
 * RENAMED 2026-09-26 from call_hover_click: renamed with its procedure; the region control's: MSInitRegion (a Mac-build name) subclasses every
 * region window with region_wndproc, and SRegion's five callbacks at +0x1a..+0x2a are the five this
 * procedure fires. It was named for its behaviour before its class was known.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_region_click(SRegion *ctl, int id)
{
    if (ctl->on_click != 0)
        ((void (*)(int))ctl->on_click)(id);
}
