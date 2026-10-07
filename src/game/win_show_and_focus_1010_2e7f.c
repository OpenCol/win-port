/* win_show_and_focus_1010_2e7f: candidate for 1010:2e7f (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Shows the window and then focuses whichever of its controls is marked:
 * `MSShowWindow_of_1010_29c1` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */
extern void MSShowWindow_of_1010_29c1(void *o);

void win_show_and_focus_1010_2e7f(DialogWin *o)
{
    MSShowWindow_of_1010_29c1(o);
    DialogWin_SetDefaultControlFocus(o);
}
