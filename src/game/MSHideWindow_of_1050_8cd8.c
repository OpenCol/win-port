/* MSHideWindow_of_1050_8cd8: candidate for 1050:8cd8 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSHideWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSHideWindow(void *win);

void MSHideWindow_of_1050_8cd8(DialogWin *o)
{
    MSHideWindow((MSWin *)o->win);
}
