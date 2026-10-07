/* MSShowWindow_of_1030_ef7f: candidate for 1030:ef7f (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1030_ef7f(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
