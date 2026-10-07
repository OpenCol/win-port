/* get_hpage: candidate for 1070:9fb3 (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_hpage
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x78 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * The horizontal page size: both window procedures add it on SB_PAGEDOWN and subtract it on
 * SB_PAGEUP in their WM_HSCROLL arm, and nothing else reads it. It was get_w_78; renamed 2026-09-26.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_hpage(EventWin *obj)
{
    return obj->hpage;
}
