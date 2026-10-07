/* win_center_1048_d80b: candidate for 1048:d80b (37 bytes).  Verdict: RELOCATABLE.
 *
 * MSCenterWindow on the window's handle (the pointer at +6), offsets 0, 0.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSCenterWindow(void *win, int dx, int dy);

void win_center_1048_d80b(DialogWin *o)
{
    MSCenterWindow((MSWin *)o->win, 0, 0);
}
