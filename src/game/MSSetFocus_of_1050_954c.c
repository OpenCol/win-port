/* MSSetFocus_of_1050_954c: candidate for 1050:954c (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetFocus` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetFocus(void *win);

void MSSetFocus_of_1050_954c(DialogWin *o)
{
    MSSetFocus((MSWin *)o->win);
}
