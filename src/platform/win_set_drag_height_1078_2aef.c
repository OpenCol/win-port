/* win_set_drag_height_1078_2aef: candidate for 1078:2aef (35 bytes).  Verdict: EXACT.
 *
 * Stores a word at +0x1e of the window record, for a non-null window.
 *
 * RENAMED 2026-09-26 from win_set_word_1e_1078_2aef: sets the window record's drag band (+0x1e): for a window created with flag 0x400,
 * main_window_message_handler answers WM_NCHITTEST with HTCAPTION for a client hit above this many
 * pixels (0 = the whole client area), so the window can be dragged by its top. ColChild_InitCWin_style_ncparent sets 32.
 */

#include "colonization.h"

#include "records.h"

void win_set_drag_height_1078_2aef(MSWin *win, int drag_height)
{
    if (win == 0)
        return;
    win->drag_height = drag_height;
}
