/* scroll_get_range_ex_1070_604d: candidate for 1070:604d (52 bytes).  Verdict: RELOCATABLE.
 *
 * GetScrollRange on the control's window handle (+2): bar 1 when `flag`
 * is set, else 0, the range through the two pointers.
 */

#include "colonization.h"

#include "records.h"

void scroll_get_range_ex_1070_604d(MSWin *win, int *lo, int *hi, int flag)
{
    GetScrollRange(win->hwnd, flag != 0, lo, hi);
}
