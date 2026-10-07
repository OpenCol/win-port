/* call_region_enter: candidate for 1070:96de (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_region_enter
 *
 * Invokes the callback at +0x1a of the region control record passed in, with one word
 * argument, when one is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_26_1070_96de: calls the region's +0x1a callback when the pointer first
 * arrives: region_wndproc takes the capture on WM_MOUSEMOVE and fires it with the control id.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 *
 * RENAMED 2026-09-26 from call_hover_enter: renamed with its procedure; the region control's: MSInitRegion (a Mac-build name) subclasses every
 * region window with region_wndproc, and SRegion's five callbacks at +0x1a..+0x2a are the five this
 * procedure fires. It was named for its behaviour before its class was known.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_region_enter(SRegion *ctl, int id)
{
    if (ctl->on_enter != 0)
        ((void (*)(int))ctl->on_enter)(id);
}
