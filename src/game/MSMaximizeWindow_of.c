/* MSMaximizeWindow_of: candidate for 1048:e1b5 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSMaximizeWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSMaximizeWindow(void *win);

void MSMaximizeWindow_of(Screen *o)
{
    MSMaximizeWindow((MSWin *)o->win);
}
