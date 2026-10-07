/* win_set_back_color_1060_85da: candidate for 1060:85da (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the background colour of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetBackground(void *win, int index);

void win_set_back_color_1060_85da(DialogWin *o, int index)
{
    MSSetBackground((MSWin *)o->win, index);
}
