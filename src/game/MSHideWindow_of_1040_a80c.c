/* PORT: this file's own declarations of MSHideWindow removed: the header declares it from the definition (W1 step 3). */
/* MSHideWindow_of_1040_a80c: candidate for 1040:a80c (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSHideWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSHideWindow_of_1040_a80c(DialogWin *o)
{
    MSHideWindow((MSWin *)o->win);
}
