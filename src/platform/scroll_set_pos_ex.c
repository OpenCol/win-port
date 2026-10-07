/* scroll_set_pos_ex: candidate for 1070:6081 (45 bytes).  Verdict: RELOCATABLE.
 *
 * SetScrollPos on the control's window handle (+2): the bar is 1 when
 * `flag` is set, the position `pos`, redraw on.
 */

#include "colonization.h"

#include "records.h"

void scroll_set_pos_ex(MSWin *win, int pos, int flag)
{
    SetScrollPos(win->hwnd, flag != 0, pos, 1);
}
