/* MSShowWindow_of_1010_29c1: candidate for 1010:29c1 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1010_29c1(Screen *o)
{
    MSShowWindow((MSWin *)o->win);
}
