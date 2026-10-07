/* call_bar_tracking: candidate for 1070:9b19 (38 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: call_bar_tracking
 *
 * Invokes handler slot +30 (0x1e) of the widget layer's handler table with one word
 * argument when a handler is installed.
 *
 * RENAMED 2026-09-26 from call_handler_slot_30_1070_9b19: calls the scrollbar's +0x1e callback with the thumb position on
 * SB_THUMBTRACK, before the position is committed.
 * The body is the generic null-checked call; each copy is named by the one control class that calls it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void call_bar_tracking(SScrollBar *bar, int value)
{
    if (bar->on_track != 0)
        ((void (*)(int))bar->on_track)(value);
}
