/* PORT: this file's own declarations of MSSetFocus removed: the header declares it from the definition (W1 step 3). */
/* MSSetFocus_of_1040_b07b: candidate for 1040:b07b (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `MSSetFocus` on the window the object holds at +6 -- the same one-line shape as
 * `win_set_focus`.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void MSSetFocus_of_1040_b07b(DialogWin *o)
{
    MSSetFocus((MSWin *)o->win);
}
