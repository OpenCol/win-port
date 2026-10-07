/* win_set_edge_width: candidate for 1078:2b12 (35 bytes).  Verdict: EXACT.
 *
 * Stores a word at +0x20 of the window record, for a non-null window; the
 * sibling of `win_set_drag_height_1078_2aef`.
 *
 * RENAMED 2026-09-26 from win_set_word_20: sets the window record's edge width (+0x20, default 5): for a window created with flag 0x1000,
 * main_window_message_handler turns a client hit within this many pixels of an edge into the matching
 * sizing hit code (HTLEFT..HTBOTTOMRIGHT), so the window resizes from its edges.
 */

#include "colonization.h"

#include "records.h"

void win_set_edge_width(MSWin *win, int edge_width)
{
    if (win == 0)
        return;
    win->edge_width = edge_width;
}
