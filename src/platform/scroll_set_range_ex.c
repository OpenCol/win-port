/* scroll_set_range_ex: candidate for 1070:6007 (70 bytes).  Verdict: RELOCATABLE.
 *
 * SetScrollRange on the control's window (+2), bar 1 when `flag` is set,
 * with redraw, then the position set to the low end through
 * scroll_set_pos_ex.
 */

#include "colonization.h"

#include "records.h"

extern void scroll_set_pos_ex(MSWin *win, int pos, int flag);

void scroll_set_range_ex(MSWin *win, int lo, int hi, int flag)
{
    SetScrollRange(win->hwnd, flag != 0, lo, hi, 1);
    scroll_set_pos_ex(win, lo, flag);
}
