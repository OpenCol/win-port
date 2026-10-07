/* call_bar_changed: candidate for 1070:9af3 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_bar_changed
 *
 * Invokes the callback at +26 (0x1a) of the scrollbar record it is given with one word
 * argument when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_26_1070_9af3: calls the scrollbar's +0x1a callback with the new position after
 * a line, page or thumb-position move (scrollbar_message).
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_bar_changed(SScrollBar *bar, int pos)
{
    if (bar->on_change != 0)
        ((void (*)(int))bar->on_change)(pos);
}
