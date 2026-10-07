/* win_set_focus: candidate for 1060:67c3 (24 bytes).  Verdict: RELOCATABLE.
 *
 * Gives the window's handle (pointer at +6) the input focus.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetFocus(void *hwnd);

void win_set_focus(DialogWin *w)
{
    MSSetFocus((MSWin *)w->win);
}
