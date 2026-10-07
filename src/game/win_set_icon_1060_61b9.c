/* PORT: this file's own declarations of win_set_icon_1078_2832 removed: the header declares it from the definition (W1 step 3). */
/* win_set_icon_1060_61b9: candidate for 1060:61b9 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the icon of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_icon_1060_61b9(DialogWin *o, int icon)
{
    win_set_icon_1078_2832((MSWin *)o->win, icon);
}
