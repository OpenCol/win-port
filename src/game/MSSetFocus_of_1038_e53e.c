/* MSSetFocus_of_1038_e53e: candidate for 1038:e53e (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetFocus` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetFocus(void *win);

void MSSetFocus_of_1038_e53e(DialogWin *o)
{
    MSSetFocus((MSWin *)o->win);
}
