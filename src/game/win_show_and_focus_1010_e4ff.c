/* win_show_and_focus_1010_e4ff: candidate for 1010:e4ff (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Sets the focus to the window and then focuses whichever of its controls is marked:
 * `MSSetFocus_of_1010_eb8f` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */
extern void MSSetFocus_of_1010_eb8f(void *o);

void win_show_and_focus_1010_e4ff(DialogWin *o)
{
    MSSetFocus_of_1010_eb8f(o);
    DialogWin_SetDefaultControlFocus(o);
}
