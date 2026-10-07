/* win_set_timer: candidate for 1070:a116 (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SetTimer(o->hwnd, id, elapse, NULL)`: starts a timer on the window whose handle is at +2
 * of the object it is given, with no callback, so the timer arrives as WM_TIMER.
 * `win_kill_timer` immediately after this stops it.
 *
 * Nothing calls either function, so which record they receive is not shown. hwnd-at-+2 is
 * MSWin's layout in 113 declarations -- the widget layer's window object, and this is the
 * widget segment -- but the name says only what the call proves.
 */

#include "colonization.h"

#include "records.h"

void win_set_timer(MSWin *win, unsigned id, unsigned elapse)
{
    SetTimer(win->hwnd, id, elapse, 0);
}
