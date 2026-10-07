/* get_ctable_1070_97b7: candidate for 1070:97b7 (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_ctable_1070_97b7
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The pointer at +0x2 of the record. A one-line accessor: 20-odd bytes of prologue and epilogue around a single move.
 *
 * RENAMED 2026-09-26 from get_p_2_1070_97b7: the window's colour table (CTable). Its result goes to
 * GRPort_ColorSync, which pushes it into the DC when its seed changed, to ctable_hpal for
 * SelectPalette, and through identity_ptr to palette_get_entry_rgb; every caller passes a window or port.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void *get_ctable_1070_97b7(Screen *r)
{
    return r->ctable;
}
