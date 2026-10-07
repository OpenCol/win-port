/* win_set_title: candidate for 1048:e5cf (39 bytes).  Verdict: RELOCATABLE.
 *
 * MSSetWinTitle on the window's handle (the pointer at +6), with two arguments.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
extern void MSSetWinTitle(MSWin *win, char *s);

void win_set_title(DialogWin *o, char *s)
{
    MSSetWinTitle((MSWin *)o->win, s);
}
