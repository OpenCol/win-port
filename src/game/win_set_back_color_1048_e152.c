/* win_set_back_color_1048_e152: candidate for 1048:e152 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the background colour of the window the object holds at +6.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetBackground(void *win, int index);

void win_set_back_color_1048_e152(Screen *o, int index)
{
    MSSetBackground((MSWin *)o->win, index);
}
