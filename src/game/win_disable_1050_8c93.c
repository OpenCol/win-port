/* win_disable_1050_8c93: candidate for 1050:8c93 (43 bytes).  Verdict: RELOCATABLE.
 *
 * Sets the byte at +0xc and disables the window (MSEnableWindow, 0).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSEnableWindow(void *win, int on);

void win_disable_1050_8c93(DialogWin *o)
{
    o->disabled = 1;
    MSEnableWindow((MSWin *)o->win, 0);
}

