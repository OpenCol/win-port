/* win_show_and_focus_1060_6b25: candidate for 1060:6b25 (41 bytes).  Verdict: RELOCATABLE.
 * symbol: win_show_and_focus_1060_6b25
 *
 * MSShowWindow_of_1060_6ce9 on the window, then DialogWin_SetDefaultControlFocus.  The first is -called
 * and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from close_dialog_1060_6b25: the body shows the window (MSShowWindow_of_*: ShowWindow SW_SHOW + BringWindowToTop; four copies take the focus through MSSetFocus_of_*) and then focuses the marked control; nothing closes. It joins the win_show_and_focus family, which already has both shapes.
 */

#include "colonization.h"

extern int g_stub_a;
#include "classes.h"
extern void DialogWin_SetDefaultControlFocus(DialogWin *o);

extern void MSShowWindow_of_1060_6ce9(void *w);

void win_show_and_focus_1060_6b25(DialogWin *w)
{
    MSShowWindow_of_1060_6ce9(w);
    DialogWin_SetDefaultControlFocus(w);
}

