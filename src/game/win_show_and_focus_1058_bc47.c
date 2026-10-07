/* PORT: this file's own declarations of MSSetFocus_of_1058_c508 removed: the header declares it from the definition (W1 step 3). */
/* win_show_and_focus_1058_bc47: candidate for 1058:bc47 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Sets the focus to the window and then focuses whichever of its controls is marked:
 * `MSSetFocus_of_1058_c508` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */

void win_show_and_focus_1058_bc47(DialogWin *o)
{
    MSSetFocus_of_1058_c508(o);
    DialogWin_SetDefaultControlFocus(o);
}
