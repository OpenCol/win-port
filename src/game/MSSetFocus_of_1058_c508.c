/* PORT: this file's own declarations of MSSetFocus removed: the header declares it from the definition (W1 step 3). */
/* MSSetFocus_of_1058_c508: candidate for 1058:c508 (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetFocus` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSSetFocus_of_1058_c508(DialogWin *o)
{
    MSSetFocus((MSWin *)o->win);
}
