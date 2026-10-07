/* win_enable_1050_82e1: candidate for 1050:82e1 (43 bytes).  Verdict: RELOCATABLE.
 *
 * Clears the byte at +0xc and enables the window (MSEnableWindow, 1).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSEnableWindow(void *win, int on);

void win_enable_1050_82e1(DialogWin *o)
{
    o->disabled = 0;
    MSEnableWindow((MSWin *)o->win, 1);
}

