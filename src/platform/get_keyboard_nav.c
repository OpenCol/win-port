/* get_keyboard_nav: candidate for 1070:9889 (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_keyboard_nav
 *
 * The byte at +0x98 of the window record, returned without promotion.
 *
 * RENAMED 2026-09-26 from byte_at_98: the byte gates keyboard navigation. dialog_wndproc walks focus on Tab/Shift+Tab only when it is
 * set, focus_at_or_after returns early when it is clear, and the checkbox, radio, image- and
 * bitmap-button procedures draw the focus frame and let Space press the control only when their
 * window's byte is set. Every game window's Init stores 0 into it.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

char get_keyboard_nav(DialogWin *r)
{
    return r->keyboard_nav;
}
