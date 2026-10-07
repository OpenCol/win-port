/* PORT: this file's own declarations of MSRestoreWindow removed: the header declares it from the definition (W1 step 3). */
/* MSRestoreWindow_of: candidate for 1060:6809 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSRestoreWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSRestoreWindow_of(DialogWin *o)
{
    MSRestoreWindow((MSWin *)o->win);
}
