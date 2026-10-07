/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* DlgControl_dtor_1040_b177: candidate for 1040:b177 (115 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DlgControl::~DlgControl()
 *
 * The module-1040 copy of DlgControl_dtor_1050_92a3.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DialogWin, DlgControl removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* DialogWin : EventWin, flattened for C */



DlgControl::~DlgControl()
{
    if (win != 0)
        DialogWin_RemoveControl((DialogWin *)win, id);
    next = 0;
    MSCloseWindow(this);
}
