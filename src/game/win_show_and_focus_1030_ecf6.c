/* PORT: this file's own declarations of MSShowWindow_of_1030_ef7f removed: the header declares it from the definition (W1 step 3). */
/* win_show_and_focus_1030_ecf6: candidate for 1030:ecf6 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Shows the window and then focuses whichever of its controls is marked:
 * `MSShowWindow_of_1030_ef7f` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */

void win_show_and_focus_1030_ecf6(DialogWin *o)
{
    MSShowWindow_of_1030_ef7f(o);
    DialogWin_SetDefaultControlFocus(o);
}
