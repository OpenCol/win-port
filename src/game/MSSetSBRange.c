/* MSSetSBRange: candidate for 1060:66c6 (35 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `scroll_set_range` on the window handle the control record holds at +0x10, with the
 * caller's two words passed through. One of FOUR functions of this shape, differing
 * only in which widget-layer routine they reach -- the control record and the +0x10
 * handle are the same in all four.
 *
 * The callee is a call into segment 1070 and is `extern`.
 *
 * RENAMED 2026-09-27 from ctl_scroll_range: Mac MSSetSBRange(ControlRecord**, short, short). It takes the control record, as the Mac does, and passes control+0x10 to the hwnd-level scroll_set_range in 1070, so it takes the MS name (the control_hide/show convention). Callers: PediaWin_Show and open_colonopedia_ex (= ShowModal), which are Mac MSSetSBRange's 2 callers exactly. Layout: Windows ctl_scroll_range, control_set_value adjacent; Mac MSSetSBRange, MSSetSBValue adjacent. Arity 3 = (control, lo, hi). A singleton, so no suffix.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void MSSetSBRange(DlgControl *control, int lo, int hi)
{
    scroll_set_range(control->hwnd, lo, hi);
}
