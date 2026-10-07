/* get_bar_pos: candidate for 1070:9ada (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_bar_pos
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x26 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_26_1070_9ada: the scrollbar's position. scrollbar_message starts from it,
 * and every arm except SB_THUMBTRACK (which only reports the tracking value through call_bar_tracking) stores the new position back into the same +0x26 field (bar->pos).
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_bar_pos(SScrollBar *bar)
{
    return bar->pos;
}
