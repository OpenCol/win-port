/* get_bar_page: candidate for 1070:9b3f (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_bar_page
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x22 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_w_22: the scrollbar's page size: scrollbar_message moves the position by
 * it on SB_PAGEUP and SB_PAGEDOWN, where the line arms move by one.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_bar_page(SScrollBar *bar)
{
    return bar->page;
}
