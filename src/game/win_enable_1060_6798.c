/* win_enable_1060_6798: candidate for 1060:6798 (43 bytes).  Verdict: RELOCATABLE.
 *
 * Clears the byte at +0xc and enables the window (MSEnableWindow, 1).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSEnableWindow(void *win, int on);

void win_enable_1060_6798(DialogWin *o)
{
    o->disabled = 0;
    MSEnableWindow((MSWin *)o->win, 1);
}
