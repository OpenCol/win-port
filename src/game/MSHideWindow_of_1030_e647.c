/* MSHideWindow_of_1030_e647: candidate for 1030:e647 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSHideWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSHideWindow(void *win);

void MSHideWindow_of_1030_e647(DialogWin *o)
{
    MSHideWindow((MSWin *)o->win);
}
