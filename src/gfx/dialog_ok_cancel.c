/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of EventWin_EndModal removed: the header declares it from the definition (W1 step 3). */
/* dialog_ok_cancel: candidate for 1068:cb6d (75 bytes).  Verdict: RELOCATABLE.
 *
 * The OK (0x65) / Cancel (0x66) handler of the dialog whose owner is kept at DGROUP:0x56b6:
 * Cancel first sets the result at DGROUP:0x56b4 to -1; both close the owner's window
 * (+0x26) through EventWin_EndModal.
 */

#include "colonization.h"

#include "classes.h"


void dialog_ok_cancel(int code)
{
    if (code == 0x65)
        EventWin_EndModal((EventWin *)(g_dialog_owner + PORT_DIALOG_BASE));
    else if (code == 0x66) {
        g_dialog_result = -1;
        EventWin_EndModal((EventWin *)(g_dialog_owner + PORT_DIALOG_BASE));
    }
}
