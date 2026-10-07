/* PORT: this file's own declarations of win_set_drag_height_1078_2aef removed: the header declares it from the definition (W1 step 3). */
/* win_set_drag_height_1048_d714: candidate for 1048:d714 (36 bytes).  Verdict: RELOCATABLE.
 *
 * Writes the word at +0x1e of the window the object holds at +6.
 *
 * RENAMED 2026-09-26 from win_set_word_1e_1048_d714: sets the window record's drag band (+0x1e): for a window created with flag 0x400,
 * main_window_message_handler answers WM_NCHITTEST with HTCAPTION for a client hit above this many
 * pixels (0 = the whole client area), so the window can be dragged by its top. ColChild_InitCWin_style_ncparent sets 32.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void win_set_drag_height_1048_d714(DialogWin *o, int drag_height)
{
    win_set_drag_height_1078_2aef((MSWin *)o->win, drag_height);
}
