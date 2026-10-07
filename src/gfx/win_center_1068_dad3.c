/* win_center_1068_dad3: candidate for 1068:dad3 (37 bytes).  Verdict: RELOCATABLE.
 *
 * MSCenterWindow on the window's handle (the pointer at +6), offsets 0, 0.
 *
 * TRANSFERRED: byte-identical to `win_center_1048_d80b` (1048:d80b), one of 12 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSCenterWindow(void *win, int dx, int dy);

void win_center_1068_dad3(DialogWin *o)
{
    MSCenterWindow((MSWin *)o->win, 0, 0);
}
