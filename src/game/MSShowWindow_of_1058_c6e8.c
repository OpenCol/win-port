/* MSShowWindow_of_1058_c6e8: candidate for 1058:c6e8 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1058_c6e8(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
