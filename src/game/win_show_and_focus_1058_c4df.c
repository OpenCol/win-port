/* win_show_and_focus_1058_c4df: candidate for 1058:c4df (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Shows the window and then focuses whichever of its controls is marked:
 * `MSShowWindow_of_1058_c6e8` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */
extern void MSShowWindow_of_1058_c6e8(void *o);

void win_show_and_focus_1058_c4df(DialogWin *o)
{
    MSShowWindow_of_1058_c6e8(o);
    DialogWin_SetDefaultControlFocus(o);
}
