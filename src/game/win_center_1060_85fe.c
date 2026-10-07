/* win_center_1060_85fe: candidate for 1060:85fe (37 bytes).  Verdict: RELOCATABLE.
 *
 * MSCenterWindow on the window's handle (the pointer at +6), offsets 0, 0.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSCenterWindow(void *win, int dx, int dy);

void win_center_1060_85fe(DialogWin *o)
{
    MSCenterWindow((MSWin *)o->win, 0, 0);
}
