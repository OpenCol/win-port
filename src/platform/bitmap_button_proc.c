/* PORT: lParam's words as Borland's 16-bit casts took them -- i16 for coordinates, sizes and positions, u16 for handles, ids and codes; (unsigned)/(int) keep 32 bits here (W4). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 93, 94, 127, 136, 137, 143 of the imported file. */
/* PORT: this file's own declarations of SetEventWindow, ctl_id_1070_96c5, ctl_rect, ctl_window_1070_96a8, get_keyboard_nav removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 96, 97, 130, 139, 140, 146 of the imported file. */
/* PORT: this file's own declarations of call_button_action, get_button_enabled_1070_981e removed: the header declares them from the definition (W1 step 3). */
/* bitmap_button_proc: candidate for 1070:1e81 (1251 bytes).  Verdict: RELOCATABLE.
 * name-source: kept from the symbol map
 *
 * THE BITMAP BUTTON'S WINDOW PROCEDURE -- the GDI sibling of `image_button_proc`, which blits
 * WinG ports. This one holds three HBITMAPs in the window's object: up at +8, down at +0xa and DISABLED
 * at +0xc, and it is the only one of the two that has a disabled look.
 *
 * ENABLED is the control's +0x1a (`get_button_enabled_1070_981e`): a disabled button ignores the mouse, paints the
 * grey bitmap, and gives up the focus if it has it. Otherwise press, capture, hit-test while the mouse
 * moves, and on the button up fire handler slot 28 if the pointer is still inside. Space does the same
 * after waiting for the key to come up, which it does by polling `GetAsyncKeyState` once round the
 * message pump.
 *
 * PAINT blits the chosen bitmap through a compatible DC, and the focus arm draws TWO rectangles with the
 * stock brush: the client rectangle and the same inset by four -- the focus frame.
 *
 * A null object answers 0, where the WinG sibling answers 1.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#include "classes.h"
#include "records.h"


/* same object: 4-byte `push cs; call ` (rule 47) */




long bitmap_button_proc(unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    unsigned memdc;
    unsigned old;
    unsigned bmp;
    int hot;
    RECT r;
    POINT pt;
    CtrlBlock *o;
    char *ctl;
    unsigned brush;
    RECT cr;
    unsigned key;
    PAINTSTRUCT ps;
    register unsigned hwnd_;
    register unsigned hdc;

    hwnd_ = hwnd;
    o = object_from_hwnd(hwnd_);
    if (o == 0)
        return 0L;
    ctl = (char *)o->ctl;
    switch (msg) {
    case WM_LBUTTONDOWN:
        if (get_button_enabled_1070_981e((SButton *)ctl) == 0)
            break;
        o->pressed = 1;
        SetFocus(hwnd_);
        SetCapture(hwnd_);
    case WM_MOUSEMOVE:
        if (o->pressed != 0) {
            pt.x = (i16)lp;
            pt.y = (int)((unsigned long)lp >> 16);
            r = *(RECT *)ctl_rect(ctl);
            OffsetRect(&r, -r.left, -r.top);
            if (PtInRect(&r, pt) == 0)
                hot = 0;
            else
                hot = 1;
            if (o->hot != hot) {
                o->hot = hot;
                InvalidateRect(hwnd_, &r, 0);
            }
        }
        break;
    case WM_LBUTTONUP:
        if (o->pressed != 0) {
            ReleaseCapture();
            o->pressed = 0;
            pt.x = (i16)lp;
            pt.y = (int)((unsigned long)lp >> 16);
            r = *(RECT *)ctl_rect(ctl);
            OffsetRect(&r, -r.left, -r.top);
            InvalidateRect(hwnd_, &r, 0);
            o->hot = 0;
            if (PtInRect(&r, pt)) {
                bitmap_button_proc(hwnd_, 0xf, 0, 0L);
                SetEventWindow(ctl_window_1070_96a8((DlgControl *)((char *)((DlgControl *)ctl))));
                call_button_action((SButton *)ctl, ctl_id_1070_96c5((DlgControl *)((char *)((DlgControl *)ctl))));
            }
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd_, &ps);
        r = *(RECT *)ctl_rect(ctl);
        ValidateRect(GetParent(hwnd_), &r);
        if (get_button_enabled_1070_981e((SButton *)ctl) != 0) {
            if (o->hot == 0)
                bmp = o->bmp0;
            if (o->hot == 1)
                bmp = o->bmp1;
        } else {
            if (GetFocus() == hwnd_)
                SetFocus(0);
            bmp = o->bmp2;
        }
        memdc = CreateCompatibleDC(hdc);
        RealizePalette(memdc);
        old = SelectObject(memdc, bmp);
        BitBlt(hdc, 0, 0, rect_width_1070_962a(&r), rect_height_1070_9649(&r), memdc, 0, 0,
               0xcc0020L);
        SelectObject(memdc, old);
        DeleteDC(memdc);
        EndPaint(hwnd_, &ps);
        if (GetFocus() == hwnd_)
            bitmap_button_proc(hwnd_, 7, 0, 0L);
        break;
    case WM_KEYDOWN:
        if (wp == VK_TAB) {
            SendMessage(GetParent(hwnd_), msg, wp, lp);
        } else if (wp == VK_SPACE && get_button_enabled_1070_981e((SButton *)ctl) != 0
                   && get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8((DlgControl *)((char *)((DlgControl *)ctl))))) != 0) {
            o->hot = 1;
            repaint_hwnd(hwnd_);
            o->hot = 0;
            key = GetAsyncKeyState(0x20);
            MSDoAllTasks_1070_9837();
            while ((char)(key >> 8) != 0)
                key = GetAsyncKeyState(0x20);
            repaint_hwnd(hwnd_);
            SetEventWindow(ctl_window_1070_96a8((DlgControl *)((char *)((DlgControl *)ctl))));
            call_button_action((SButton *)ctl, ctl_id_1070_96c5((DlgControl *)((char *)((DlgControl *)ctl))));
        } else {
            SendMessage(GetParent(hwnd_), msg, wp, lp);
        }
        break;
    case WM_SETFOCUS:
        if (get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8((DlgControl *)((char *)((DlgControl *)ctl))))) == 0)
            return control_message_handler(hwnd_, msg, wp, lp);
        hdc = GetDC(hwnd_);
        brush = GetStockObject(3);
        GetClientRect(hwnd_, &cr);
        SelectObject(hdc, brush);
        FrameRect(hdc, &cr, brush);
        SetRect(&cr, cr.left + 4, cr.top + 4, cr.right - 4, cr.bottom - 4);
        FrameRect(hdc, &cr, brush);
        DeleteObject(brush);
        ReleaseDC(hwnd_, hdc);
        break;
    case WM_KILLFOCUS:
        repaint_hwnd(hwnd_);
        break;
    default:
        return control_message_handler(hwnd_, msg, wp, lp);
    }
    return 0L;
}
