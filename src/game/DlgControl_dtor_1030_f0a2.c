/* DlgControl_dtor_1030_f0a2: candidate for 1030:f0a2 (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DlgControl::~DlgControl()
 *
 * The module-1030 copy of DlgControl_dtor_1050_92a3.
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
