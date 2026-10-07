/* PORT: this file's own declarations of DialogWin_RemoveControl removed: the header declares it from the definition (W1 step 3). */
/* DlgControl_ctor_1008_ec05: candidate for 1050:9389 (143 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DlgControl::DlgControl()
 *
 * This module's copy of the control base constructor (0x1a bytes): id and win cleared, an
 * empty rectangle, hwnd and next link cleared, initial_focus and the hot-key character
 * cleared, and hotkey_pos = -1 (no hot key).
 */

#include "colonization.h"

#include "w16types.h"


#include "classes.h"
DlgControl::DlgControl()
{
    id = 0;
    win = 0;
    SetRect(&rect, 0, 0, 0, 0);
    hwnd = 0;
    next = 0;
    initial_focus = 0;
    hotkey = 0;
    hotkey_pos = -1;
}
