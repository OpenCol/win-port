/* MSShowWindow_of_1020_b106: candidate for 1020:b106 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1020_b106(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
