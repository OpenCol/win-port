/* ctl_move: candidate for 1048:d830 (35 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `move_hwnd_keeping_size` on the window handle the control record holds at +0x10, with the
 * caller's two words passed through. One of FOUR functions of this shape, differing
 * only in which widget-layer routine they reach -- the control record and the +0x10
 * handle are the same in all four.
 *
 * The callee is a call into segment 1070 and is `extern`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void ctl_move(DlgControl *control, int x, int y)
{
    move_hwnd_keeping_size(control->hwnd, x, y);
}
