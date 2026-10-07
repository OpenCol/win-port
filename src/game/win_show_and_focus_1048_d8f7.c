/* win_show_and_focus_1048_d8f7: candidate for 1048:d8f7 (41 bytes).  Verdict: RELOCATABLE.
 * symbol: win_show_and_focus_1048_d8f7
 *
 * MSSetFocus_of_1048_e0cd on the window, then DialogWin_SetDefaultControlFocus.  The first is -called
 * and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from close_dialog_1048_d8f7: the body shows the window (MSShowWindow_of_*: ShowWindow SW_SHOW + BringWindowToTop; four copies take the focus through MSSetFocus_of_*) and then focuses the marked control; nothing closes. It joins the win_show_and_focus family, which already has both shapes.
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

extern void MSSetFocus_of_1048_e0cd(Screen *w);

void win_show_and_focus_1048_d8f7(DialogWin *w)
{
    MSSetFocus_of_1048_e0cd((Screen *)w);
    DialogWin_SetDefaultControlFocus(w);
}

