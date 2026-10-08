/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 169, 177 of the imported file. */
/* PORT: this file's own declarations of SetEventWindow, ctl_id_1070_96c5, ctl_rect, ctl_window_1070_96a8, font_handle_1070_9806, get_ctable_1070_97b7, get_keyboard_nav, palette_get_entry_rgb removed: the header declares them from the definition (W1 step 3). */
/* checkbox_wndproc: candidate for 1070:31e1 (1694 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: checkbox_wndproc
 * name-source: descriptive
 * size-source: 1694, not the census 1683. The last eleven bytes are a dead second epilogue, which is
 * what a `RETF 0x0A` frame costs (rule 44); `split_underscores` starts after them.
 *
 * THE CHECK BOX'S WINDOW PROCEDURE, and the widget `checkbox_click` clicks from outside.
 * The window object carries three bitmaps -- greyed at +8, unchecked at +0xa, checked at +0xc -- and
 * its label at +0x14, with the control record itself at +2. It is the GDI sibling of
 * `bitmap_button_proc`, which holds up/down/disabled in the same three slots for a bitmap button.
 *
 * THE UNDERLINED ACCELERATOR is what makes this one long. The control's +0x18 is the index of the
 * character to underline, or -1 for none, and the underline is measured rather than drawn by the
 * font: the label is copied, truncated one character BEFORE the accelerator and measured with
 * DT_CALCRECT to get its left edge, copied again, truncated one character AFTER it and measured
 * again to get its right edge. The two edges are then thrown away -- the midpoint is taken, five
 * pixels are subtracted, and a FIXED TEN-PIXEL bar is drawn there, twice, at bottom-3 and bottom-2.
 * So the underline is always ten pixels wide whatever the character is, and the whole measuring pass
 * only decides where to centre it.
 *
 * MOUSE: a button down toggles the checked word at +0x1e, takes the focus and the capture, and fires
 * handler slot 26 with the id and the new state -- the same sequence `checkbox_click` runs,
 * open-coded here. DGROUP:0x33c4 is the guard: it is set on the way down and cleared on the button
 * up, and while it is set a second button down does nothing at all. Space does the same as a click,
 * by sending itself the two mouse messages, but only when the control's window has its +0x98 keyboard-nav byte set; tab and
 * everything else go to the parent.
 *
 * DISABLED (+0x20 zero) paints the grey bitmap and GIVES UP THE FOCUS -- `SetFocus(0)`, to nothing,
 * not to the parent -- from inside WM_PAINT.
 *
 * THE FOCUS FRAME is drawn outside WM_PAINT: the paint arm ends by testing whether this window has
 * the focus and, if it does, FALLS THROUGH into WM_SETFOCUS, which takes its own DC and frames the
 * client rectangle inset by the 32-pixel box column.
 *
 * TWO SHIPPED ODDITIES. `GetBitmapDimensionEx` is called on the chosen bitmap and its SIZE is never
 * read. And a second rectangle is built beside the first -- 32 wide, its top vertically centred but its bottom at the control's top + 20 --
 * only when the capture flag is set, and nothing ever draws it: a pressed-box highlight that was
 * computed and then dropped. Both are dead stores that -Od faithfully emits.
 *
 * The pen is `GetStockObject(DKGRAY_BRUSH)`: a BRUSH selected where a pen is wanted, so the two
 * underline bars come out in the default black pen, and the same brush is what frames the focus
 * rectangle -- where it is the right kind of object.
 *
 * WHAT THE BYTES SETTLED: the text colour is the WINDOWS.H `RGB` macro exactly as it is written
 * there -- red and green combined in SIXTEEN bits and only then widened, with blue shifted in as a
 * dword -- which is why `MOVZX EAX,AX` sits between the two halves. The colour index comes from
 * SEG34:0x2add when the control is enabled and 0x2ade when it is not, both zero-extended, so both
 * are `unsigned char` here where their setters take a plain `char` (rule 9).
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SButton, SImageButton, SRegion, SScrollBar, SCheckBox, SRadioSet, SEditBox, SListBox removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-


class DlgControl;












#define RGB(r,g,b) ((long)(((unsigned char)(r) | ((unsigned)(unsigned char)(g) << 8)) \
                           | (((unsigned long)(unsigned char)(b)) << 16)))

#include "records.h"


/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int get_checked_1070_9921(SCheckBox *ctl);
extern "C" void set_checked_1070_993a(SCheckBox *check, int checked);
extern "C" int get_check_enabled_1070_997b(SCheckBox *check);


extern "C" void call_checkbox_changed(SCheckBox *ctl, int id, int checked);

extern "C" long checkbox_wndproc(unsigned hwnd, unsigned msg, unsigned wp, intptr_t lp)
{
    register unsigned hdc;
    register unsigned w;
    unsigned char r, green, blue;
    CtrlBlock *o;
    SCheckBox *ctl;
    RECT rc;
    RECT box;
    unsigned bmp;
    unsigned saved;
    FontRec *font;
    char *text;
    SIZE dim;
    unsigned pen;
    RECT mr;
    int x1;
    int x2;
    int wide;
    PAINTSTRUCT ps;
    char buf[0x100];

    w = hwnd;
    o = object_from_hwnd(w);
    ctl = (SCheckBox *)o->ctl;
    switch (msg) {
    case WM_LBUTTONDOWN:
        if (g_checkbox_capture == 0) {
            SetFocus(w);
            if (get_checked_1070_9921(ctl) == 0)
                set_checked_1070_993a(ctl, 1);
            else
                set_checked_1070_993a(ctl, 0);
            g_checkbox_capture = 1;
            SetCapture(w);
            InvalidateRect(w, 0, 0);
            SetEventWindow(ctl_window_1070_96a8(ctl));
            call_checkbox_changed(ctl, ctl_id_1070_96c5(ctl), get_checked_1070_9921(ctl));
        }
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        g_checkbox_capture = 0;
        break;
    case WM_PAINT:
        if (get_check_enabled_1070_997b(ctl)) {
            if (get_checked_1070_9921(ctl))
                bmp = o->bmp2;
            else
                bmp = o->bmp1;
        } else {
            if (GetFocus() == w)
                SetFocus(0);
            bmp = o->bmp0;
        }
        GetBitmapDimensionEx(bmp, &dim);
        rc = *(RECT *)ctl_rect((char *)ctl);
        box = rc;
        if (g_checkbox_capture != 0) {
            box.right = box.left + 0x20;
            box.top = (rect_height_1070_9649(&rc) - 20) / 2 + rc.top;
            box.bottom = rc.top + 20;
        }
        OffsetRect(&rc, -rc.left, -rc.top);
        hdc = BeginPaint(w, &ps);
        wing_child_paint(&ps, w);
        font = (FontRec *)MSHLock(font_handle_1070_9806((Font *)g_default_font_ref));
        saved = SelectObject(hdc, font->hfont);
        MSHUnlock(font_handle_1070_9806((Font *)g_default_font_ref));
        SetBkMode(hdc, TRANSPARENT);
        blit_transparent(hdc, bmp, rc.left, (rc.bottom - 20) / 2, 0x20, 0x14, 0, 7);
        rc.left += 0x25;
        text = (char *)o->data;
        if (get_check_enabled_1070_997b(ctl) == 1) {
            if (g_check_color_enabled != 0) {
                palette_get_entry_rgb(identity_ptr_1070_97ee(
                                          get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(ctl)))),
                                      g_check_color_enabled, (char*)(&r), (char*)(&green), (char*)(&blue));
                SetTextColor(hdc, RGB(r, green, blue));
            } else
                SetTextColor(hdc, 0L);
        } else if (get_check_enabled_1070_997b(ctl) == 0) {
            if (g_check_color_disabled != 0) {
                palette_get_entry_rgb(identity_ptr_1070_97ee(
                                          get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(ctl)))),
                                      g_check_color_disabled, (char*)(&r), (char*)(&green), (char*)(&blue));
                SetTextColor(hdc, RGB(r, green, blue));
            } else
                SetTextColor(hdc, 0L);
        }
        DrawText(hdc, text, STRLEN(text), &rc, DT_VCENTER | DT_SINGLELINE);
        if (ctl->hotkey_pos != -1) {
            STRCPY(buf, text);
            buf[ctl->hotkey_pos] = 0;
            mr = rc;
            if (STRLEN(buf) == 0)
                x1 = mr.left;
            else {
                DrawText(hdc, buf, STRLEN(buf), &mr, DT_CALCRECT | DT_VCENTER | DT_SINGLELINE);
                x1 = mr.right - 1;
            }
            STRCPY(buf, text);
            buf[ctl->hotkey_pos + 1] = 0;
            mr = rc;
            DrawText(hdc, buf, STRLEN(buf), &mr, DT_CALCRECT | DT_VCENTER | DT_SINGLELINE);
            x2 = mr.right + 1;
            wide = x2 - x1;
            x1 = wide / 2 + x1 - 5;
            x2 = x1 + 10;
            pen = GetStockObject(DKGRAY_BRUSH);
            SelectObject(hdc, pen);
            MoveToEx(hdc, x1, rc.bottom - 3, 0);
            LineTo(hdc, x2, rc.bottom - 3);
            MoveToEx(hdc, x1, rc.bottom - 2, 0);
            LineTo(hdc, x2, rc.bottom - 2);
        }
        SelectObject(hdc, saved);
        EndPaint(w, &ps);
        if (GetFocus() != w)
            break;
        /* falls through */
    case WM_SETFOCUS:
        if (get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8(ctl))) == 0)
            return control_message_handler(w, msg, wp, lp);
        hdc = GetDC(w);
        pen = GetStockObject(DKGRAY_BRUSH);
        GetClientRect(w, &rc);
        SelectObject(hdc, pen);
        rc.left += 0x20;
        FrameRect(hdc, &rc, pen);
        DeleteObject(pen);
        ReleaseDC(w, hdc);
        break;
    case WM_KEYDOWN:
        if (wp == VK_TAB)
            SendMessage(GetParent(w), msg, wp, lp);
        else if (wp == VK_SPACE && get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8(ctl))) != 0) {
            SendMessage(w, WM_LBUTTONDOWN, 0, 0L);
            SendMessage(w, WM_LBUTTONUP, 0, 0L);
        } else
            SendMessage(GetParent(w), msg, wp, lp);
        break;
    case WM_KILLFOCUS:
        repaint_hwnd(w);
        break;
    default:
        return control_message_handler(w, msg, wp, lp);
    }
    return 0;
}
