/* MSShowWindow_of_1040_b3ca: candidate for 1040:b3ca (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSShowWindow(void *win);

void MSShowWindow_of_1040_b3ca(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
