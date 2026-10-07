/* win_enable_1008_d809: candidate for 1008:d809 (43 bytes).  Verdict: RELOCATABLE.
 *
 * Clears the byte at +0xc and enables the window (MSEnableWindow, 1).
 *
 * TRANSFERRED: byte-identical to `win_enable_1050_82e1` (1050:82e1), one of 5 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSEnableWindow(void *win, int on);

void win_enable_1008_d809(Screen *o)
{
    o->disabled = 0;
    MSEnableWindow((MSWin *)o->win, 1);
}

