/* get_vpos: candidate for 1070:9fcc (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_vpos
 * name-source: descriptive
 * size-source: 26, not the census 22. The trailing 4 bytes are the dead second epilogue.
 *
 * The `int` at +0x8a of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * The vertical scroll position: both window procedures read it at the top of their WM_VSCROLL arm and
 * store the new position back through the same field. It was get_w_8a; renamed 2026-09-26.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int get_vpos(EventWin *obj)
{
    return obj->vpos;
}
