/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 99, 100, 116, 117, 132, 135, 143, 156, 164, 165, 171 of the imported file. */
/* PORT: this file's own declarations of GRPort_GetWin_1070_960d, call_image_button_click, call_image_button_rclick, ctable_hpal, port_height_1070_98bc, port_width_1070_98a3 removed: the header declares them from the definition (W1 step 3). */
/* image_button_proc: candidate for 1070:2782 (1579 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 1579, not the census 1117. The census stops before the last arms; the five-entry jump
 * table for messages 0x201..0x205 is the last thing in the function and `text_row_rect`
 * starts right after it.
 *
 * THE IMAGE BUTTON'S WINDOW PROCEDURE, installed by `MSInitImageButton`. The window's object carries the
 * pressed flag at +0x10 and the "drawn pressed" flag at +0xe, and the control itself hangs off +2 with
 * its up port at +0x22 and its down port at +0x48.
 *
 * MOUSE: a left or right button down takes the focus and the capture and marks it pressed. While pressed,
 * every mouse move hit-tests the control's rectangle and repaints when the answer changes, so the button
 * pops out when the pointer leaves it. The button UP releases the capture, repaints unpressed, and fires
 * the handler only if the pointer is still inside -- slot 26 for the left button and slot 30 for the
 * right. A double click falls through to the default handler.
 *
 * PAINT: the rectangle is validated in the parent as well as here, the control's palette is selected and
 * realised, and the up or down port is blitted with WinG. The DOWN port's blit passes its WIDTH as the
 * height -- `get_w_2` twice, where the up port uses `get_w_2` and `get_w_4` -- so a non-square pressed
 * image is drawn wrong. The paint ends by sending itself a WM_SETFOCUS when it already has the focus.
 *
 * KEYBOARD: tab is passed to the parent; space, when the owning window's +0x98 byte (`get_keyboard_nav`) is set, presses the button,
 * waits for the key to come up with `GetAsyncKeyState`, and fires slot 26. Anything else goes to the
 * parent.
 *
 * Spellings: HIWORD is an UNSIGNED shift; the hit flag is an if/else and not a ternary; the key state is
 * unsigned; and the focus test is `==`.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
#include "classes.h"
#include "records.h"

extern void MSLockGraf(GRPort *port);
extern void MSUnlockGraf(GRPort *port);
extern void GRPort_ColorSync(GRPort *port, CTable *table);
extern void SetEventWindow(void *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void *ctl_rect(char *ctl);
extern void *ctl_window_1070_96a8(char *ctl);
extern void *get_ctable_1070_97b7(void *c);
extern int ctl_id_1070_96c5(char *ctl);
extern char get_keyboard_nav(void *o);

long image_button_proc(unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    unsigned hdc;
    int hot;
    RECT r;
    POINT pt;
    CtrlBlock *o;
    char *ctl;
    unsigned key;
    PAINTSTRUCT ps;

    o = object_from_hwnd(hwnd);
    if (o == 0)
        return 1L;
    ctl = (char *)o->ctl;
    switch (msg) {
    case WM_LBUTTONDOWN:
        o->pressed = 1;
        SetFocus(hwnd);
        SetCapture(hwnd);
    case WM_MOUSEMOVE:
        if (o->pressed != 0) {
            pt.x = (int)lp;
            pt.y = (int)((unsigned long)lp >> 16);
            r = *(RECT *)ctl_rect(ctl);
            OffsetRect(&r, -r.left, -r.top);
            if (PtInRect(&r, pt) == 0)
                hot = 0;
            else
                hot = 1;
            if (o->hot != hot) {
                o->hot = hot;
                InvalidateRect(hwnd, &r, 0);
            }
        }
        break;
    case WM_LBUTTONUP:
        if (o->pressed != 0) {
            ReleaseCapture();
            o->pressed = 0;
            pt.x = (int)lp;
            pt.y = (int)((unsigned long)lp >> 16);
            r = *(RECT *)ctl_rect(ctl);
            OffsetRect(&r, -r.left, -r.top);
            InvalidateRect(hwnd, &r, 0);
            o->hot = 0;
            if (PtInRect(&r, pt)) {
                image_button_proc(hwnd, 0xf, 0, 0L);
                SetEventWindow(ctl_window_1070_96a8((char *)((DlgControl *)ctl)));
                call_image_button_click((SImageButton *)ctl, ctl_id_1070_96c5((char *)((DlgControl *)ctl)));
            }
        }
        break;
    case WM_RBUTTONUP:
        if (o->pressed != 0) {
            ReleaseCapture();
            o->pressed = 0;
            pt.x = (int)lp;
            pt.y = (int)((unsigned long)lp >> 16);
            r = *(RECT *)ctl_rect(ctl);
            OffsetRect(&r, -r.left, -r.top);
            InvalidateRect(hwnd, &r, 0);
            o->hot = 0;
            if (PtInRect(&r, pt)) {
                image_button_proc(hwnd, 0xf, 0, 0L);
                SetEventWindow(ctl_window_1070_96a8((char *)((DlgControl *)ctl)));
                call_image_button_rclick((SImageButton *)ctl, ctl_id_1070_96c5((char *)((DlgControl *)ctl)));
            }
        }
        break;
    case WM_RBUTTONDOWN:
        o->pressed = 1;
        SetFocus(hwnd);
        SetCapture(hwnd);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        r = *(RECT *)ctl_rect(ctl);
        ValidateRect(GetParent(hwnd), &r);
        OffsetRect(&r, -r.left, -r.top);
        ValidateRect(hwnd, &r);
        SelectPalette(hdc, ctable_hpal((CTable *)(get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8((char *)((DlgControl *)ctl)))))), 0);
        if (o->hot == 0) {
            MSLockGraf((GRPort *)(ctl + 0x22));
            GRPort_ColorSync((GRPort *)(ctl + 0x22), (CTable *)get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8((char *)((DlgControl *)ctl)))));
            RealizePalette(hdc);
            WinGBitBlt(hdc, 0, 0, port_width_1070_98a3((GRPort *)(ctl + 0x22)), port_height_1070_98bc((GRPort *)(ctl + 0x22)),
                       GRPort_GetWin_1070_960d((GRPort *)(ctl + 0x22))->hdc, 0, 0);
            MSUnlockGraf((GRPort *)(ctl + 0x22));
        }
        if (o->hot == 1) {
            MSLockGraf((GRPort *)(ctl + 0x48));
            GRPort_ColorSync((GRPort *)(ctl + 0x48), (CTable *)get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8((char *)((DlgControl *)ctl)))));
            RealizePalette(hdc);
            WinGBitBlt(hdc, 0, 0, port_width_1070_98a3((GRPort *)(ctl + 0x48)), port_width_1070_98a3((GRPort *)(ctl + 0x48)),
                       GRPort_GetWin_1070_960d((GRPort *)(ctl + 0x48))->hdc, 0, 0);
            MSUnlockGraf((GRPort *)(ctl + 0x48));
        }
        EndPaint(hwnd, &ps);
        if (GetFocus() == hwnd)
            image_button_proc(hwnd, 7, 0, 0L);
        break;
    case WM_KEYDOWN:
        if (wp == VK_TAB) {
            SendMessage(GetParent(hwnd), msg, wp, lp);
        } else if (wp == VK_SPACE && get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8((char *)((DlgControl *)ctl)))) != 0) {
            o->hot = 1;
            repaint_hwnd(hwnd);
            o->hot = 0;
            key = GetAsyncKeyState(0x20);
            while ((char)(key >> 8) != 0)
                key = GetAsyncKeyState(0x20);
            repaint_hwnd(hwnd);
            SetEventWindow(ctl_window_1070_96a8((char *)((DlgControl *)ctl)));
            call_image_button_click((SImageButton *)ctl, ctl_id_1070_96c5((char *)((DlgControl *)ctl)));
        } else {
            SendMessage(GetParent(hwnd), msg, wp, lp);
        }
        break;
    case WM_SETFOCUS:
        if (get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8((char *)((DlgControl *)ctl)))) == 0)
            return 0L;
        return control_message_handler(hwnd, msg, wp, lp);
    case WM_KILLFOCUS:
        repaint_hwnd(hwnd);
        break;
    default:
        return control_message_handler(hwnd, msg, wp, lp);
    }
    return 0L;
}
