/* MSPushButton: candidate for 1070:1dbb (198 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSPushButton
 * name-source: sibling-build
 *
 * A button being clicked, from the window handle: take the focus, draw the button pressed,
 * hold it for eight ticks, draw it released, and then fire the handler.
 *
 * The visible press is two repaints around a flag: `+0xe` of the window record goes to 1,
 * the window is repainted, the flag goes back to 0, and only then is the delay taken -- so
 * the pressed state is on screen for the whole wait and the release repaint follows it. The
 * flag is cleared BEFORE the wait rather than after the second repaint, which is what makes
 * the second repaint draw the released button without a third store.
 *
 * Two early returns, each with its own epilogue: no object behind the handle, and a control
 * whose enabled word at +0x1a is zero. The first test is a 32-bit compare of the whole 
 * pointer.
 *
 * `g_current_window` (DGROUP:0x2ae0) is set to the control's owner window -- the pointer
 * stored at the control's +4 -- before the handler runs, which is how the handler finds its window
 * without being passed it.
 *
 * Every callee but `SetFocus` is in this segment and reached by the four-byte
 * `PUSH CS / CALL NEAR` form, so all of them are stubs here (rule 47).
 *
 * RENAMED 2026-09-27 from button_click: BEYOND THE PROPOSALS. It vacates button_click for 1050:31dd and replaces a descriptive name with the Mac platform name. Mac MSPushButton(ControlRecord**), CODE 6 0x0f74, 518 bytes. Its body checks the 'MSBT' tag, then HiliteControl (A95D), Delay (A03B), HiliteControl, then calls the action through a register. 1dbb (unsigned hwnd), 198 bytes: 'take the focus, draw the button pressed, hold it for eight ticks, draw it released, and then fire the handler'. Layout: Mac MSInitButton, MSDisposeButton, MSPushButton, then check-box code; Windows MSInitButton (0f45), MSDisposeButton (1d94), 1dbb, then bitmap_button_proc. Caller: Mac MacCtrlDoKeyEvent; Windows dialog_mnemonic (the key path) and dialog_wndproc. Arity: one control handle, which is an HWND on Windows. Medium only because this is platform code (the Mac draws the press with Toolbox traps; the size is 2.6x) and because there are no constants or strings. call_image_button_click has button_click as a suffix, so rename by exact word. Apply it first in its chain.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

/* same object: 4-byte `push cs; call ` */

extern "C" int get_button_enabled_1070_981e(SButton *button);
extern "C" void *ctl_window_1070_96a8(SButton *ctl);
extern "C" int ctl_id_1070_96c5(SButton *ctl);
extern "C" void call_button_action(SButton *ctl, int id);

void MSPushButton(unsigned hwnd)
{
    CtrlBlock *w;
    SButton *ctl;

    w = object_from_hwnd(hwnd);
    if (w == 0)
        return;
    ctl = (SButton *)w->ctl;
    if (get_button_enabled_1070_981e(ctl) == 0)
        return;
    SetFocus(hwnd);
    w->hot = 1;
    repaint_hwnd(hwnd);
    w->hot = 0;
    MSDoAllTasks_1070_9837();
    MSWaitTicks_1070_984a(8);
    repaint_hwnd(hwnd);
    g_current_window = ctl_window_1070_96a8(ctl);
    call_button_action(ctl, ctl_id_1070_96c5(ctl));
}
