/* PORT: this file's own declarations of MSSetFocus_of_1038_e53e removed: the header declares it from the definition (W1 step 3). */
/* win_show_and_focus_1038_e279: candidate for 1038:e279 (41 bytes).  Verdict: RELOCATABLE.
 * symbol: win_show_and_focus_1038_e279
 *
 * MSSetFocus_of_1038_e53e on the window, then DialogWin_SetDefaultControlFocus.  The first is -called
 * and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from close_dialog_1038_e279: the body shows the window (MSShowWindow_of_*: ShowWindow SW_SHOW + BringWindowToTop; four copies take the focus through MSSetFocus_of_*) and then focuses the marked control; nothing closes. It joins the win_show_and_focus family, which already has both shapes.
 */

#include "colonization.h"

extern int g_stub_a;
#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);


void win_show_and_focus_1038_e279(DialogWin *w)
{
    MSSetFocus_of_1038_e53e(w);
    DialogWin_SetDefaultControlFocus(w);
}
