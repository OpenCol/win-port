/* PORT: this file's own declarations of MSSetFocus removed: the header declares it from the definition (W1 step 3). */
/* MSSetFocus_of_1010_eb8f: candidate for 1010:eb8f (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetFocus` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSSetFocus_of_1010_eb8f(Screen *o)
{
    MSSetFocus((MSWin *)o->win);
}
