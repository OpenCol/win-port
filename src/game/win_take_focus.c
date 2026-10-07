/* win_take_focus: candidate for 1060:62f3 (39 bytes, 3 callers).  Verdict: RELOCATABLE.
 *
 * Focuses the window, then runs DialogWin_SetDefaultControlFocus over it (which walks the list at +0x8c).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
#include "classes.h"
extern void win_set_focus(DialogWin *w);
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

void win_take_focus(DialogWin *w)
{
    win_set_focus(w);
    DialogWin_SetDefaultControlFocus(w);
}
