/* win_kill_timer: candidate for 1070:a139 (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `KillTimer(o->hwnd, id)`: stops a timer `win_set_timer` (immediately before) started on the
 * window whose handle is at +2 of the object it is given.
 */

#include "colonization.h"

#include "records.h"

void win_kill_timer(MSWin *win, unsigned id)
{
    KillTimer(win->hwnd, id);
}
