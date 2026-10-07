/* PORT: this file's own declarations of MSShowWindow removed: the header declares it from the definition (W1 step 3). */
/* MSShowWindow_of_1060_8623: candidate for 1060:8623 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSShowWindow_of_1060_8623(DialogWin *o)
{
    MSShowWindow((MSWin *)o->win);
}
