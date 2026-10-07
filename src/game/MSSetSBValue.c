/* MSSetSBValue: candidate for 1060:66e9 (42 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores v at +0x26 of the control and passes it to scroll_set_pos with the
 * control's window handle.
 *
 * RENAMED 2026-09-27 from control_set_value: Mac MSSetSBValue(ControlRecord**, short). It stores the value at control+0x26 and passes control+0x10 to scroll_set_pos. Callers: PediaWin_Show and open_colonopedia_ex, which are Mac MSSetSBValue's 2 callers exactly. Layout: follows MSSetSBRange on both. Arity 2. A singleton.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void MSSetSBValue(SScrollBar *control, int pos)
{
    control->pos = pos;
    scroll_set_pos(control->hwnd, pos);
}
