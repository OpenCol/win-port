/* MSShowWindow_of_1060_6ce9: candidate for 1060:6ce9 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1060_6ce9(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
