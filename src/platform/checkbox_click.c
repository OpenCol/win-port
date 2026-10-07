/* checkbox_click: candidate for 1070:3113 (206 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: checkbox_click
 * name-source: descriptive
 *
 * `MSPushButton`'s counterpart for a check box, and the difference between them is
 * what the two widgets are: the button flashes and fires, this one TOGGLES and fires.
 *
 * The word at +0x1e of the control record is read and written back the other way round --
 * two branches with two constants, not an expression -- so +0x1e is the checked state. Then
 * the mouse is captured, the whole window invalidated rather than repainted twice, and the
 * handler called with both the value it just set and the control's id.
 *
 * The handler is slot 0x1a, which takes TWO words, where the button's slot 0x1c takes one.
 * Right-to-left evaluation is why the checked state is fetched again before the id.
 *
 * `InvalidateRect(hwnd, 0, 0)` pushes its null rectangle as one 32-bit `66 6A 00`, which is
 * what a NULL costs under -3.
 *
 * Every callee but the two USER imports is in this segment and reached by the four-byte
 * `PUSH CS / CALL NEAR` form, so all of them are stubs here (rule 47).
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

/* same object: 4-byte `push cs; call ` */

extern "C" int get_checked_1070_9921(SCheckBox *ctl);
extern "C" void set_checked_1070_993a(SCheckBox *check, int checked);
extern "C" void *ctl_window_1070_96a8(SCheckBox *ctl);
extern "C" int ctl_id_1070_96c5(SCheckBox *ctl);
extern "C" void call_checkbox_changed(SCheckBox *ctl, int id, int checked);

void checkbox_click(unsigned hwnd)
{
    CtrlBlock *w;
    SCheckBox *ctl;

    w = object_from_hwnd(hwnd);
    if (w == 0)
        return;
    ctl = (SCheckBox *)w->ctl;
    SetFocus(hwnd);
    if (get_checked_1070_9921(ctl) == 0)
        set_checked_1070_993a(ctl, 1);
    else
        set_checked_1070_993a(ctl, 0);
    SetCapture(hwnd);
    InvalidateRect(hwnd, 0, 0);
    g_current_window = ctl_window_1070_96a8(ctl);
    call_checkbox_changed(ctl, ctl_id_1070_96c5(ctl), get_checked_1070_9921(ctl));
}
