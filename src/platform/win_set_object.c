/* win_set_object: candidate for 1070:6811 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SetWindowLong(win->hwnd, 4, o)` -- binds the object to its window through GWL offset 4, so a
 * window procedure can find the C++ object behind the HWND. Its caller passes
 * `(o, get_win_1068_d72a(o))`.
 */

#include "colonization.h"

#include "records.h"

void win_set_object(void *o, MSWin *win)
{
    SetWindowLong(win->hwnd, 4, (long)o);
}
