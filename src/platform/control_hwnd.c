/* control_hwnd: candidate for 1070:a6d7 (56 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: control_hwnd
 *
 * The control's window handle (+0x10), 0 for a null control.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int control_hwnd(DlgControl *control)
{
    int h;

    if (control == 0)
        return 0;
    h = control->hwnd;
    return h;
}
