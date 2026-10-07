/* PORT: this file's own declarations of call_region_click, call_region_dblclk, call_region_enter, call_region_leave, call_region_press removed: the header declares them from the definition (W1 step 3). */
/* region_wndproc: candidate for 1070:0cc0 (645 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -a
 * symbol: region_wndproc
 * name-source: descriptive
 * size-source: 645, not the census 91. The census stops at the first `RETF 0x0A`; the function runs
 * to the four-entry jump table at 1070:0f45, where `MSDisposeButton`'s neighbour begins.
 *
 * A CONTROL THAT KNOWS WHEN THE MOUSE IS OVER IT. Four messages, two flags and five handler slots,
 * and everything else falls through to `control_message_handler`.
 *
 * THE TRICK IS THAT WM_MOUSEMOVE TAKES THE CAPTURE. The first move over the control sets the
 * "inside" flag, calls `SetCapture` and fires slot 26 -- the mouse-entered handler. Because the
 * capture is now held, every later move anywhere on screen comes back here, and the control works
 * out that the pointer has left by copying its own rectangle, sliding it to the origin
 * (`OffsetRect` by minus its own left and top, so the rect becomes 0,0-based and the message's
 * client coordinates can be tested against it) and asking `PtInRect`. On the way out it clears the
 * flag, releases the capture and fires slot 30. That is hover highlighting built entirely out of
 * mouse capture, with no WM_MOUSELEAVE -- which Windows 3.1 did not have.
 *
 * THE CAPTURE IS NOT RELEASED WHILE THE BUTTON IS DOWN. Both release paths are guarded by the
 * pressed flag, so a press pins the capture until the release, and the release then chooses
 * between slot 34 (let go while still inside) and slot 30 (let go after wandering off), which is
 * exactly the click-or-cancel decision a push button has to make.
 *
 * TWO OF THE FIVE SLOTS SKIP THE NULL CHECK. The entered, left and released handlers all test the
 * control pointer first; the pressed (slot 38) and double-clicked (slot 42) handlers do not, and
 * would fault on a window whose object carries no control. They are reachable only after a move
 * has already run, which is presumably why nobody noticed.
 *
 * WHAT THE BYTES SETTLED: the function needs `-a` (rule 81). Without it the code is byte-identical
 * but one byte shorter, because the jump table lands at an odd offset and the object does not emit
 * the pad -- the same flag, now with a -jump WORD table rather than a rule 80 dword table.
 *
 * RENAMED 2026-09-26 from hover_button_wndproc: the window procedure of MSInitRegion (a Mac-build name) subclasses every
 * region window with region_wndproc, and SRegion's five callbacks at +0x1a..+0x2a are the five this
 * procedure fires. It was named for its behaviour before its class was known.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#include "classes.h"

#include "records.h"

extern void SetEventWindow(void *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void *ctl_window_1070_96a8(void *ctl);
extern int ctl_id_1070_96c5(void *ctl);
extern RECT *ctl_rect(void *ctl);

long region_wndproc(unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    RECT r;
    POINT pt;
    CtrlBlock *w;
    void *ctl;

    w = object_from_hwnd(hwnd);
    ctl = w->ctl;
    switch (msg) {
    case WM_MOUSEMOVE:
        pt.x = (int)lp;
        pt.y = (int)((unsigned long)lp >> 16);
        if (g_hover_active == 0) {
            if (g_hover_pressed == 0) {
                g_hover_active = 1;
                SetCapture(hwnd);
                if (ctl != 0) {
                    SetEventWindow(ctl_window_1070_96a8((DlgControl *)ctl));
                    call_region_enter((SRegion *)ctl, ctl_id_1070_96c5((DlgControl *)ctl));
                }
            }
        } else {
            r = *ctl_rect((char *)ctl);
            OffsetRect(&r, -r.left, -r.top);
            if (PtInRect(&r, pt) == 0) {
                g_hover_active = 0;
                if (g_hover_pressed == 0) {
                    ReleaseCapture();
                    if (ctl != 0) {
                        SetEventWindow(ctl_window_1070_96a8((DlgControl *)ctl));
                        call_region_leave((SRegion *)ctl, ctl_id_1070_96c5((DlgControl *)ctl));
                    }
                }
            }
        }
        break;
    case WM_LBUTTONDOWN:
        if (g_hover_active != 0) {
            g_hover_pressed = 1;
            SetEventWindow(ctl_window_1070_96a8((DlgControl *)ctl));
            call_region_press((SRegion *)ctl, ctl_id_1070_96c5((DlgControl *)ctl));
        }
        break;
    case WM_LBUTTONUP:
        if (g_hover_pressed != 0) {
            g_hover_pressed = 0;
            if (g_hover_active != 0) {
                if (ctl != 0) {
                    SetEventWindow(ctl_window_1070_96a8((DlgControl *)ctl));
                    call_region_click((SRegion *)ctl, ctl_id_1070_96c5((DlgControl *)ctl));
                }
            } else {
                ReleaseCapture();
                if (ctl != 0) {
                    SetEventWindow(ctl_window_1070_96a8((DlgControl *)ctl));
                    call_region_leave((SRegion *)ctl, ctl_id_1070_96c5((DlgControl *)ctl));
                }
            }
        }
        break;
    case WM_LBUTTONDBLCLK:
        if (g_hover_active != 0) {
            SetEventWindow(ctl_window_1070_96a8((DlgControl *)ctl));
            call_region_dblclk((SRegion *)ctl, ctl_id_1070_96c5((DlgControl *)ctl));
        }
        break;
    default:
        return control_message_handler(hwnd, msg, wp, lp);
    }
    return 0;
}
