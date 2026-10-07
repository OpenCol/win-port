/* set_keyboard_nav_1010_eb29: candidate for 1010:eb29 (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a byte at +0x98 of a record. One of FIVE copies, one per module.
 *
 * RENAMED 2026-09-26 from set_b_98_1010_eb29: the byte gates keyboard navigation. dialog_wndproc walks focus on Tab/Shift+Tab only when it is
 * set, focus_at_or_after returns early when it is clear, and the checkbox, radio, image- and
 * bitmap-button procedures draw the focus frame and let Space press the control only when their
 * window's byte is set. Every game window's Init stores 0 into it.
 */

#include "colonization.h"

#include "classes.h"

void set_keyboard_nav_1010_eb29(DialogWin *win, char keyboard_nav)
{
    win->keyboard_nav = keyboard_nav;
}
