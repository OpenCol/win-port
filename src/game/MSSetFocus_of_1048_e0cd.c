/* MSSetFocus_of_1048_e0cd: candidate for 1048:e0cd (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetFocus` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetFocus(void *win);

void MSSetFocus_of_1048_e0cd(Screen *o)
{
    MSSetFocus((MSWin *)o->win);
}
