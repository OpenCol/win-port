/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 22 of the imported file. */
/* PORT: this file's own declarations of win_show removed: the header declares it from the definition (W1 step 3). */
/* win_show_and_focus_1008_dbda: candidate for 1008:dbda (41 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Shows the window and then focuses whichever of its controls is marked:
 * `win_show` followed by `DialogWin_SetDefaultControlFocus`, both on the same object.
 * One of NINE copies; the first call is this module's own and the second is shared.
 *
 * The first callee is -called and is a stub here; the second is a call and is
 * `extern` (rule 47).
 */

#include "colonization.h"

#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

/* same object: 4-byte `push cs; call ` */

void win_show_and_focus_1008_dbda(DialogWin *o)
{
    win_show((Screen *)(o));
    DialogWin_SetDefaultControlFocus(o);
}
