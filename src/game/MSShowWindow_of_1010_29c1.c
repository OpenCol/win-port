/* PORT: this file's own declarations of MSShowWindow removed: the header declares it from the definition (W1 step 3). */
/* MSShowWindow_of_1010_29c1: candidate for 1010:29c1 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSShowWindow` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSShowWindow_of_1010_29c1(Screen *o)
{
    MSShowWindow((MSWin *)o->win);
}
