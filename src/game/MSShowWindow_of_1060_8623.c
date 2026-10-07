/* MSShowWindow_of_1060_8623: candidate for 1060:8623 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1060_8623(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
