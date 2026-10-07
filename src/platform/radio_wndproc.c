/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 188, 196 of the imported file. */
/* PORT: this file's own declarations of SetEventWindow, ctl_id_1070_96c5, ctl_window_1070_96a8, font_handle_1070_9806, get_ctable_1070_97b7, get_keyboard_nav, palette_get_entry_rgb removed: the header declares them from the definition (W1 step 3). */
/* radio_wndproc: candidate for 1070:447d (2849 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: radio_wndproc
 * name-source: descriptive
 * size-source: 2849, not the census 2542. The census stops before the last arms; the function ends
 * with a dead second epilogue and then the four-entry jump table for messages 0x202..0x205, and
 * `hwnd_is_ms_control` starts right after it.
 *
 * THE RADIO BUTTON'S WINDOW PROCEDURE -- one window per item, all of them sharing the group's control
 * record. `radio_click` is the same selection change reached from outside; this is the
 * widget itself, and the two halves agree member for member.
 *
 * THE ITEM RECORD is 0x92 bytes: its HWND at +0, its rectangle at +2, its own index in the group at
 * +0xa, an enabled word at +0xc that is tested against exactly 1, its label inline at +0xe, and the
 * accelerator index at +0x90. The array of them hangs off the control at +0x2e and the selected index
 * at +0x2a, so `items[sel].hwnd` -- the same `IMUL 0x92` -- is how one item repaints another.
 *
 * KEYBOARD NAVIGATION is what makes this one long, and it is a proper circular search. Right, down,
 * numpad 6 and numpad 2 step forward; up, left, numpad 8 and numpad 4 step backward. Either way the
 * index wraps at the ends and the walk SKIPS DISABLED ITEMS, guarded by a countdown seeded with the
 * item count so an all-disabled group stops instead of spinning. Landing on an item takes the focus,
 * repaints the old selection and the new one, and fires handler slot 26 -- exactly what a click does.
 *
 * THREE HANDLER SLOTS, not one: 26 for a plain click or a keyboard move, 30 for a double click, and
 * 34 for a right button down. The right button has its own capture flag at DGROUP:0x33c8 beside the
 * left's at 0x33c6, and neither is per-window, so the whole group shares them.
 *
 * PAINTING follows `checkbox_wndproc` line for line, down to the dead `GetBitmapDimensionEx`
 * and the pressed-box rectangle that is computed and never drawn. The accelerator underline is the
 * same measure-then-ignore: two DT_CALCRECT passes find the character's edges and then a fixed
 * ten-pixel bar is centred between them. Two differences are real. The paint arm INVALIDATES THE WHOLE
 * WINDOW WITH ERASE SET before it begins painting, which is a repaint asking for another repaint. And
 * a disabled item with no colour flag falls back to 0x404040 where the check box falls back to black.
 *
 * The font is the one at DGROUP:0x2ace, not the check box's 0x2ad2, and the ink indices are SEG34's
 * 0x2adb and 0x2adc rather than 0x2add and 0x2ade -- the four flags `set_radio_color_on`
 * and its three neighbours write, split one pair to each widget.
 *
 * WHAT THE BYTES SETTLED: the label buffer is 128 bytes here where the check box's is 256, and the
 * frame is 0xe0 rather than 0x15c because of it. One scratch `int` serves as both the message's key
 * code and the underline's width -- the same slot at -0x42 in two arms -- so it is one local, not two.
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

extern "C" int get_cur_item_1070_99ac(SRadioSet *ctl);
extern "C" void set_cur_item_1070_9994(SRadioSet *radios, int cur_item);
extern "C" int radio_count_1070_9a5d(SRadioSet *ctl);
extern "C" RadioItem *get_items_1070_99c5(SRadioSet *radios);


extern "C" void call_radio_changed(SRadioSet *ctl, int id, int item);
extern "C" void call_radio_dblclk(SRadioSet *ctl, int id, int index);
extern "C" void call_radio_rbutton_down(SRadioSet *ctl, int id, int index);

extern "C" long radio_wndproc(unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    register int idx;
    register unsigned w;
    CtrlBlock *o;
    SRadioSet *ctl;
    RadioItem *item;
    FontRec *font;
    RECT rc;
    RECT box;
    RECT mr;
    unsigned bmp;
    unsigned hdc;
    unsigned saved;
    SIZE dim;
    unsigned pen;
    int x1;
    int x2;
    int count;
    unsigned char r, green, blue;
    int t;
    PAINTSTRUCT ps;
    char buf[0x80];

    w = hwnd;
    o = object_from_hwnd(w);
    ctl = (SRadioSet *)o->ctl;
    item = (RadioItem *)o->data;
    switch (msg) {
    case WM_LBUTTONDOWN:
        if (g_radio_capture == 0) {
            SetFocus(w);
            if (get_cur_item_1070_99ac(ctl) != item->index && item->enabled == 1) {
                if (get_cur_item_1070_99ac(ctl) != -1)
                    InvalidateRect(get_items_1070_99c5(ctl)[get_cur_item_1070_99ac(ctl)].hwnd, 0, 0);
                set_cur_item_1070_9994(ctl, item->index);
            }
            g_radio_capture = 1;
            SetCapture(w);
            InvalidateRect(w, 0, 0);
            SetEventWindow(ctl_window_1070_96a8(ctl));
            call_radio_changed(ctl, ctl_id_1070_96c5(ctl), get_cur_item_1070_99ac(ctl));
        }
        break;
    case WM_LBUTTONUP:
        ReleaseCapture();
        g_radio_capture = 0;
        break;
    case WM_LBUTTONDBLCLK:
        if (item->enabled == 1) {
            SetEventWindow(ctl_window_1070_96a8(ctl));
            if (get_cur_item_1070_99ac(ctl) != item->index) {
                if (get_cur_item_1070_99ac(ctl) != -1)
                    InvalidateRect(get_items_1070_99c5(ctl)[get_cur_item_1070_99ac(ctl)].hwnd, 0, 0);
                set_cur_item_1070_9994(ctl, item->index);
            }
            call_radio_dblclk(ctl, ctl_id_1070_96c5(ctl), item->index);
        }
        break;
    case WM_RBUTTONDOWN:
        if (g_radio_rcapture == 0) {
            g_radio_rcapture = 1;
            SetCapture(w);
            SetEventWindow(ctl_window_1070_96a8(ctl));
            call_radio_rbutton_down(ctl, ctl_id_1070_96c5(ctl), item->index);
        }
        break;
    case WM_RBUTTONUP:
        ReleaseCapture();
        g_radio_rcapture = 0;
        break;
    case WM_PAINT:
        if (item->enabled == 1) {
            if (get_cur_item_1070_99ac(ctl) == item->index)
                bmp = o->bmp2;
            else
                bmp = o->bmp1;
        } else
            bmp = o->bmp0;
        GetBitmapDimensionEx(bmp, &dim);
        rc = item->rc;
        box = rc;
        if (g_radio_capture != 0) {
            box.right = box.left + 0x20;
            box.top = (rect_height_1070_9649(&rc) - 20) / 2 + rc.top;
            box.bottom = rc.top + 20;
        }
        InvalidateRect(w, 0, 1);
        OffsetRect(&rc, -rc.left, -rc.top);
        hdc = BeginPaint(w, &ps);
        wing_child_paint(&ps, w);
        font = (FontRec *)MSHLock(font_handle_1070_9806((Font *)g_control_font));
        saved = SelectObject(hdc, font->hfont);
        MSHUnlock(font_handle_1070_9806((Font *)g_control_font));
        SetBkMode(hdc, TRANSPARENT);
        blit_transparent(hdc, bmp, rc.left, (rc.bottom - 20) / 2, 0x20, 0x14, 0, 7);
        rc.left += 0x25;
        if (item->enabled == 1) {
            if (g_radio_color_enabled != 0) {
                palette_get_entry_rgb(identity_ptr_1070_97ee(
                                          get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(ctl)))),
                                      g_radio_color_enabled, (char*)(&r), (char*)(&green), (char*)(&blue));
                SetTextColor(hdc, RGB(r, green, blue));
            } else
                SetTextColor(hdc, 0L);
        } else if (item->enabled == 0) {
            if (g_radio_color_disabled != 0) {
                palette_get_entry_rgb(identity_ptr_1070_97ee(
                                          get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(ctl)))),
                                      g_radio_color_disabled, (char*)(&r), (char*)(&green), (char*)(&blue));
                SetTextColor(hdc, RGB(r, green, blue));
            } else
                SetTextColor(hdc, 0x404040L);
        }
        DrawText(hdc, item->label, STRLEN(item->label), &rc, DT_VCENTER | DT_SINGLELINE);
        if (item->accel != -1) {
            STRCPY(buf, item->label);
            buf[item->accel] = 0;
            mr = rc;
            if (STRLEN(buf) == 0)
                x1 = mr.left;
            else {
                DrawText(hdc, buf, STRLEN(buf), &mr, DT_CALCRECT | DT_VCENTER | DT_SINGLELINE);
                x1 = mr.right - 1;
            }
            STRCPY(buf, item->label);
            buf[item->accel + 1] = 0;
            mr = rc;
            DrawText(hdc, buf, STRLEN(buf), &mr, DT_CALCRECT | DT_VCENTER | DT_SINGLELINE);
            x2 = mr.right + 1;
            t = x2 - x1;
            x1 = t / 2 + x1 - 5;
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
        t = wp;
        if (t == VK_TAB)
            SendMessage(GetParent(w), msg, wp, lp);
        else if (t == VK_SPACE && get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8(ctl))) != 0) {
            SendMessage(w, WM_LBUTTONDOWN, 0, 0L);
            SendMessage(w, WM_LBUTTONUP, 0, 0L);
        } else if ((t == VK_RIGHT || t == VK_DOWN || t == VK_NUMPAD6 || t == VK_NUMPAD2)
                   && get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8(ctl))) != 0) {
            count = radio_count_1070_9a5d(ctl);
            idx = item->index + 1;
            if (idx >= radio_count_1070_9a5d(ctl))
                idx = 0;
            while (count > 0 && get_items_1070_99c5(ctl)[idx].enabled == 0) {
                count--;
                idx++;
                if (idx >= radio_count_1070_9a5d(ctl))
                    idx = 0;
            }
            if (count <= 0)
                break;
            SetFocus(get_items_1070_99c5(ctl)[idx].hwnd);
            InvalidateRect(get_items_1070_99c5(ctl)[get_cur_item_1070_99ac(ctl)].hwnd, 0, 0);
            set_cur_item_1070_9994(ctl, idx);
            InvalidateRect(get_items_1070_99c5(ctl)[idx].hwnd, 0, 0);
            SetEventWindow(ctl_window_1070_96a8(ctl));
            call_radio_changed(ctl, ctl_id_1070_96c5(ctl), get_cur_item_1070_99ac(ctl));
        } else if ((t == VK_UP || t == VK_LEFT || t == VK_NUMPAD8 || t == VK_NUMPAD4)
                   && get_keyboard_nav((DialogWin *)(ctl_window_1070_96a8(ctl))) != 0) {
            count = radio_count_1070_9a5d(ctl);
            idx = item->index - 1;
            if (idx < 0)
                idx = radio_count_1070_9a5d(ctl) - 1;
            while (count > 0 && get_items_1070_99c5(ctl)[idx].enabled == 0) {
                count--;
                idx--;
                if (idx < 0)
                    idx = radio_count_1070_9a5d(ctl) - 1;
            }
            if (count <= 0)
                break;
            SetFocus(get_items_1070_99c5(ctl)[idx].hwnd);
            InvalidateRect(get_items_1070_99c5(ctl)[get_cur_item_1070_99ac(ctl)].hwnd, 0, 0);
            set_cur_item_1070_9994(ctl, idx);
            InvalidateRect(get_items_1070_99c5(ctl)[idx].hwnd, 0, 0);
            SetEventWindow(ctl_window_1070_96a8(ctl));
            call_radio_changed(ctl, ctl_id_1070_96c5(ctl), get_cur_item_1070_99ac(ctl));
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
