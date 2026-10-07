/* PORT: this file's own declarations of MSSetFocus removed: the header declares it from the definition (W1 step 3). */
/* win_set_focus: candidate for 1060:67c3 (24 bytes).  Verdict: RELOCATABLE.
 *
 * Gives the window's handle (pointer at +6) the input focus.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_focus(DialogWin *w)
{
    MSSetFocus((MSWin *)w->win);
}
