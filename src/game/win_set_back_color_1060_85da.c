/* PORT: this file's own declarations of MSSetBackground removed: the header declares it from the definition (W1 step 3). */
/* win_set_back_color_1060_85da: candidate for 1060:85da (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the background colour of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_back_color_1060_85da(DialogWin *o, int index)
{
    MSSetBackground((MSWin *)o->win, index);
}
