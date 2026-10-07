/* win_set_cursor_1060_6275: candidate for 1060:6275 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the cursor of the window the object holds at +6, from the word the pointer holds and one more argument.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void win_set_cursor_1078_28e4(void *win, int cur, int now);

void win_set_cursor_1060_6275(DialogWin *o, int *p, int now)
{
    win_set_cursor_1078_28e4((MSWin *)o->win, *p, now);
}
