/* DlgControl_dtor_1008_ec94: candidate for 1008:ec94 (115 bytes, 4 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DlgControl::~DlgControl()
 *
 * The destructor of DlgControl as compiled into module 1008: unlink from the parent's
 * control list by id, clear the next link, close the window. A copy of DlgControl_dtor_1050_92a3.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DialogWin, DlgControl removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* DialogWin : EventWin, flattened for C */
extern void DialogWin_RemoveControl(void *list, int id);



DlgControl::~DlgControl()
{
    if (win != 0)
        DialogWin_RemoveControl((DialogWin *)win, id);
    next = 0;
    MSCloseWindow(this);
}
