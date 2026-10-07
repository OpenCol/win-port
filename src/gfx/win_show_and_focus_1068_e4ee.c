/* win_show_and_focus_1068_e4ee: candidate for 1068:e4ee (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Shows the window and then focuses whichever of its controls is marked:
 * `MSShowWindow_of_1068_e66f` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */
extern void MSShowWindow_of_1068_e66f(void *o);

void win_show_and_focus_1068_e4ee(DialogWin *o)
{
    MSShowWindow_of_1068_e66f(o);
    DialogWin_SetDefaultControlFocus(o);
}
