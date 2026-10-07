/* win_set_icon_1048_e191: candidate for 1048:e191 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the icon of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void win_set_icon_1078_2832(void *win, int icon);

void win_set_icon_1048_e191(Screen *o, int icon)
{
    win_set_icon_1078_2832((MSWin *)o->win, icon);
}
