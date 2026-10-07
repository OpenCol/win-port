/* win_disable_1008_d766: candidate for 1008:d766 (43 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the byte at +0xc and disables the window (MSEnableWindow, 0).
 *
 * TRANSFERRED: byte-identical to `win_disable_1050_8c93` (1050:8c93), one of 4 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSEnableWindow(void *win, int on);

void win_disable_1008_d766(Screen *o)
{
    o->disabled = 1;
    MSEnableWindow((MSWin *)o->win, 0);
}

