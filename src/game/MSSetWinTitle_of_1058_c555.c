/* MSSetWinTitle_of_1058_c555: candidate for 1058:c555 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetWinTitle` on the window the object holds at +6, with the caller's pointer passed
 * through -- the same one-line shape as `win_set_focus`, one argument wider.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
extern void MSSetWinTitle(MSWin *win, const char *s);

void MSSetWinTitle_of_1058_c555(DialogWin *o, const char *title)
{
    MSSetWinTitle((MSWin *)o->win, title);
}
