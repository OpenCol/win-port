/* PORT: this file's own declarations of ctable_hpal, get_win_1070_95f0, parse_hotkey_marker removed: the header declares them from the definition (W1 step 3). */
/* MSInitButton: candidate for 1070:0f45 (3663 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSInitButton
 * name-source: original
 * size-source: 3663, not the census 3654. The written-out `return hwnd` leaves a dead second
 * epilogue of nine bytes after the live one (rule 44); `MSDisposeButton` starts after them.
 *
 * MAKES A PUSH BUTTON AND PAINTS ALL THREE OF ITS FACES ONCE. The control itself is a plain
 * MSControlClass child window like `MSInitCheck`'s; what makes this 3,654 bytes is that the
 * up, down and greyed pictures are drawn here, into three compatible bitmaps, and the window
 * procedure afterwards only ever blits whichever one the state calls for. Nothing is drawn
 * again while the button lives.
 *
 * THE THREE PASSES are written out three times in the source rather than looped, and the
 * differences between them are exactly what a Windows 3.1 button looks like:
 *   UP    -- white along the top and left, grey along the bottom and right, two pixels deep
 *            each, label centred.
 *   DOWN  -- grey on the top and left only, no highlight at all, and the label
 *            rectangle's left and top edges moved in two pixels (right and bottom stay put), so
 *            the centred text lands one pixel right and down and appears pressed in.
 *   GREY  -- the up border again, but the label drawn twice: white first, then grey one
 *            pixel up and left, which is the classic engraved disabled text.
 *
 * THE FACE COLOUR is either the light-grey stock brush, when the object's colour index is -1,
 * or a pen made from the palette entry that index names -- and in the second case the
 * rectangle is filled by drawing one horizontal line per scanline rather than by `FillRect`,
 * because a pen is not a brush. The same `if` therefore appears at every fill and again at the
 * end, where the pen is deleted and the stock brush is not.
 *
 * THE HOT KEY is underlined by measurement, not by mark-up. The label is copied twice into a
 * scratch buffer and cut short -- once before the hot-key character and once after it -- and
 * each truncated copy is measured with DT_CALCRECT. The two right edges bracket the character;
 * the underline is a ten-pixel bar centred between them, drawn twice one pixel apart. When the
 * hot key is the FIRST character the left cut gives an empty string, so the left edge falls
 * back to the rectangle's own left.
 *
 * FAILING TO MAKE ANY OF THE THREE BITMAPS destroys the window and returns 0, and all three
 * arms report the same sentence -- "Could not create bitmap in MSInitButton" -- through the
 * module's own empty reporter. The string is in the data segment three times over.
 *
 * WHAT THE BYTES SETTLED, three spellings:
 *   The colour is built by the SDK's own `RGB` macro and not by three independent casts. The
 *   macro combines red and green in SIXTEEN bits (`MOV AH,0 / SHL DX,8 / OR AX,DX`) and only
 *   then widens (`MOVZX EAX,AX`), where casting each component to `unsigned long` separately
 *   widens all three.
 *   `o = obj` is a STATEMENT, not an initialiser: the shipped copy happens after the label
 *   `STRCPY`, and an initialiser would run before every statement in the body.
 *   And the underline's `SelectObject` result is THROWN AWAY -- the restore at the end of the
 *   hot-key block puts back the pen saved by the border drawing further up, not one saved
 *   here. Writing `oldpen =` there adds a store the shipped does not have.
 *
 * `g_button_font` (SEG34:0x2aca) is what this reads. `run_game` sets it at startup
 * with `set_button_font_1008_d033(&g_number_font)`, and `MSInitButton` is its only reader --
 * the font a button's own label is measured and drawn in. Its counterpart `g_control_font`
 * (0x2ace) is set per popup from `p->font` by `Popup_make_controls` and read by
 * `radio_wndproc` and `static_text_wndproc`.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
/* PORT: this file's own Font, CTable, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SButton, SImageButton, SRegion, SScrollBar, SCheckBox, SRadioSet, SEditBox, SListBox removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
   /* 0x41e bytes: the palette, its handle and the cycling state */
#pragma option -a

#pragma option -a-


class DlgControl;











#include "records.h"

extern "C" void *ctl_window_1070_96a8(void *ctl);
extern "C" void report_error_1070_969a(void *ctx, char *msg);
extern "C" void *get_ctable_1070_97b7(void *o);

extern "C" void palette_get_entry_rgb(void *pal, int i, unsigned char *r, unsigned char *g, unsigned char *b);
extern "C" int font_handle_1070_9806(void *ref);
extern "C" CtrlBlock *InitCtrlBlock(unsigned hwnd, void *obj);

extern "C" unsigned MSInitButton(RECT *rgn, SButton *obj, const char *text,
                                     int visible)
{
    register unsigned memdc;
    register int y;
    unsigned hwnd;
    CtrlBlock *c;
    RECT rc;
    unsigned oldbmp;
    unsigned bmp_up;
    unsigned bmp_down;
    unsigned bmp_grey;
    unsigned hdc;
    unsigned face;
    unsigned oldpen;
    unsigned white;
    unsigned grey;
    unsigned *fp;
    unsigned oldfont;
    SButton *button;
    RECT tr;
    RECT save;
    RECT keep;
    int w;
    int x0;
    int x1;
    unsigned char red, green, blue;
    unsigned long style;
    char part[0x100];
    char label[0x100];

    STRCPY(label, text);
    button = obj;
    style = 0x40810000L;
    if (visible)
        style |= WS_VISIBLE;
    hwnd = CreateWindow("MSControlClass", "", style, rgn->left, rgn->top,
                        rect_width_1070_962a(rgn), rect_height_1070_9649(rgn),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8(obj)))->hwnd, 0, g_hinstance, 0);
    parse_hotkey_marker(button, label);
    rc = *rgn;
    OffsetRect(&rc, -rc.left, -rc.top);
    rc.right -= 2;
    rc.bottom -= 2;
    hdc = GetDC(hwnd);
    bmp_up = CreateCompatibleBitmap(hdc, rect_width_1070_962a(&rc), rect_height_1070_9649(&rc));
    if (bmp_up == 0) {
        report_error_1070_969a(&g_error_log, "Could not create bitmap in MSInitButton");
        DestroyWindow(hwnd);
        return 0;
    }
    bmp_down = CreateCompatibleBitmap(hdc, rect_width_1070_962a(&rc), rect_height_1070_9649(&rc));
    if (bmp_down == 0) {
        report_error_1070_969a(&g_error_log, "Could not create bitmap in MSInitButton");
        DestroyWindow(hwnd);
        return 0;
    }
    bmp_grey = CreateCompatibleBitmap(hdc, rect_width_1070_962a(&rc), rect_height_1070_9649(&rc));
    if (bmp_grey == 0) {
        report_error_1070_969a(&g_error_log, "Could not create bitmap in MSInitButton");
        DestroyWindow(hwnd);
        return 0;
    }
    memdc = CreateCompatibleDC(hdc);
    ReleaseDC(hwnd, hdc);
    SetBkMode(memdc, TRANSPARENT);
    if (button->colour == -1)
        face = GetStockObject(LTGRAY_BRUSH);
    else {
        SelectPalette(memdc, ctable_hpal((CTable *)(get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(button))))), 0);
        RealizePalette(memdc);
        palette_get_entry_rgb(identity_ptr_1070_97ee(get_ctable_1070_97b7((Screen *)(ctl_window_1070_96a8(button)))),
                              button->colour, &red, &green, &blue);
        face = CreatePen(PS_SOLID, 1,
                         (unsigned long)(unsigned)(red | ((unsigned)green << 8))
                         | ((unsigned long)blue << 16));
    }
    white = CreatePen(PS_SOLID, 1, 0xffffffL);
    grey = CreatePen(PS_SOLID, 1, 0x808080L);
    fp = (unsigned *)MSHLock(font_handle_1070_9806((Font *)g_button_font));
    oldfont = SelectObject(memdc, *fp);
    oldbmp = SelectObject(memdc, bmp_up);
    keep = rc;
    if (button->colour == -1)
        FillRect(memdc, &rc, face);
    else {
        oldpen = SelectObject(memdc, face);
        for (y = rc.top; y < rc.bottom; y++) {
            MoveToEx(memdc, rc.left, y, 0);
            LineTo(memdc, rc.right, y);
        }
        SelectObject(memdc, oldpen);
    }
    oldpen = SelectObject(memdc, white);
    MoveToEx(memdc, rc.left, rc.bottom, 0);
    LineTo(memdc, rc.left, rc.top);
    LineTo(memdc, rc.right, rc.top);
    MoveToEx(memdc, rc.left + 1, rc.bottom - 1, 0);
    LineTo(memdc, rc.left + 1, rc.top + 1);
    LineTo(memdc, rc.right - 1, rc.top + 1);
    SelectObject(memdc, grey);
    MoveToEx(memdc, rc.left + 1, rc.bottom - 1, 0);
    LineTo(memdc, rc.right - 1, rc.bottom - 1);
    LineTo(memdc, rc.right - 1, rc.top);
    MoveToEx(memdc, rc.left + 2, rc.bottom - 2, 0);
    LineTo(memdc, rc.right - 2, rc.bottom - 2);
    LineTo(memdc, rc.right - 2, rc.top + 1);
    SelectObject(memdc, oldpen);
    tr = rc;
    DrawText(memdc, label, STRLEN(label), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
    w = rect_width_1070_962a(&tr);
    SetRect(&tr, rect_width_1070_962a(&rc) / 2 + rc.left - w / 2, rc.top,
            rect_width_1070_962a(&rc) / 2 + rc.left - w / 2 + w, rc.bottom);
    DrawText(memdc, label, STRLEN(label), &tr, DT_SINGLELINE | DT_VCENTER);
    save = tr;
    if (button->hotkey_pos >= 0) {
        STRCPY(part, label);
        part[button->hotkey_pos] = 0;
        if (STRLEN(part) != 0) {
            tr = save;
            DrawText(memdc, part, STRLEN(part), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
            x0 = tr.right - 1;
        } else
            x0 = tr.left;
        STRCPY(part, label);
        part[button->hotkey_pos + 1] = 0;
        tr = save;
        DrawText(memdc, part, STRLEN(part), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
        x1 = tr.right + 1;
        w = x1 - x0;
        x0 = w / 2 + x0 - 5;
        x1 = x0 + 10;
        SelectObject(memdc, grey);
        MoveToEx(memdc, x0, tr.bottom - 2, 0);
        LineTo(memdc, x1, tr.bottom - 2);
        MoveToEx(memdc, x0, tr.bottom - 1, 0);
        LineTo(memdc, x1, tr.bottom - 1);
        SelectObject(memdc, oldpen);
    }
    SelectObject(memdc, bmp_down);
    if (button->colour == -1)
        FillRect(memdc, &rc, face);
    else {
        oldpen = SelectObject(memdc, face);
        for (y = rc.top; y < rc.bottom; y++) {
            MoveToEx(memdc, rc.left, y, 0);
            LineTo(memdc, rc.right, y);
        }
        SelectObject(memdc, oldpen);
    }
    oldpen = SelectObject(memdc, grey);
    MoveToEx(memdc, rc.left, rc.bottom, 0);
    LineTo(memdc, rc.left, rc.top);
    LineTo(memdc, rc.right, rc.top);
    MoveToEx(memdc, rc.left + 1, rc.bottom - 1, 0);
    LineTo(memdc, rc.left + 1, rc.top + 1);
    LineTo(memdc, rc.right - 1, rc.top + 1);
    SelectObject(memdc, oldpen);
    rc.left += 2;
    rc.top += 2;
    tr = rc;
    DrawText(memdc, label, STRLEN(label), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
    w = rect_width_1070_962a(&tr);
    SetRect(&tr, rect_width_1070_962a(&rc) / 2 + rc.left - w / 2, rc.top,
            rect_width_1070_962a(&rc) / 2 + rc.left - w / 2 + w, rc.bottom);
    DrawText(memdc, label, STRLEN(label), &tr, DT_SINGLELINE | DT_VCENTER);
    save = tr;
    if (button->hotkey_pos >= 0) {
        STRCPY(part, label);
        part[button->hotkey_pos] = 0;
        if (STRLEN(part) != 0) {
            tr = save;
            DrawText(memdc, part, STRLEN(part), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
            x0 = tr.right - 1;
        } else
            x0 = tr.left;
        STRCPY(part, label);
        part[button->hotkey_pos + 1] = 0;
        tr = save;
        DrawText(memdc, part, STRLEN(part), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
        x1 = tr.right + 1;
        w = x1 - x0;
        x0 = w / 2 + x0 - 5;
        x1 = x0 + 10;
        SelectObject(memdc, grey);
        MoveToEx(memdc, x0, tr.bottom - 2, 0);
        LineTo(memdc, x1, tr.bottom - 2);
        MoveToEx(memdc, x0, tr.bottom - 1, 0);
        LineTo(memdc, x1, tr.bottom - 1);
        SelectObject(memdc, oldpen);
    }
    SelectObject(memdc, bmp_grey);
    rc = keep;
    if (button->colour == -1)
        FillRect(memdc, &rc, face);
    else {
        oldpen = SelectObject(memdc, face);
        for (y = rc.top; y < rc.bottom; y++) {
            MoveToEx(memdc, rc.left, y, 0);
            LineTo(memdc, rc.right, y);
        }
        SelectObject(memdc, oldpen);
    }
    oldpen = SelectObject(memdc, white);
    MoveToEx(memdc, rc.left, rc.bottom, 0);
    LineTo(memdc, rc.left, rc.top);
    LineTo(memdc, rc.right, rc.top);
    MoveToEx(memdc, rc.left + 1, rc.bottom - 1, 0);
    LineTo(memdc, rc.left + 1, rc.top + 1);
    LineTo(memdc, rc.right - 1, rc.top + 1);
    SelectObject(memdc, grey);
    MoveToEx(memdc, rc.left + 1, rc.bottom - 1, 0);
    LineTo(memdc, rc.right - 1, rc.bottom - 1);
    LineTo(memdc, rc.right - 1, rc.top);
    MoveToEx(memdc, rc.left + 2, rc.bottom - 2, 0);
    LineTo(memdc, rc.right - 2, rc.bottom - 2);
    LineTo(memdc, rc.right - 2, rc.top + 1);
    SelectObject(memdc, oldpen);
    tr = rc;
    DrawText(memdc, label, STRLEN(label), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
    w = rect_width_1070_962a(&tr);
    SetRect(&tr, rect_width_1070_962a(&rc) / 2 + rc.left - w / 2, rc.top,
            rect_width_1070_962a(&rc) / 2 + rc.left - w / 2 + w, rc.bottom);
    SetTextColor(memdc, 0xffffffL);
    DrawText(memdc, label, STRLEN(label), &tr, DT_SINGLELINE | DT_VCENTER);
    OffsetRect(&tr, -1, -1);
    SetTextColor(memdc, 0x808080L);
    DrawText(memdc, label, STRLEN(label), &tr, DT_SINGLELINE | DT_VCENTER);
    OffsetRect(&tr, 1, 1);
    save = tr;
    if (button->hotkey_pos >= 0) {
        STRCPY(part, label);
        part[button->hotkey_pos] = 0;
        if (STRLEN(part) != 0) {
            tr = save;
            DrawText(memdc, part, STRLEN(part), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
            x0 = tr.right - 1;
        } else
            x0 = tr.left;
        STRCPY(part, label);
        part[button->hotkey_pos + 1] = 0;
        tr = save;
        DrawText(memdc, part, STRLEN(part), &tr, DT_CALCRECT | DT_SINGLELINE | DT_VCENTER);
        x1 = tr.right + 1;
        w = x1 - x0;
        x0 = w / 2 + x0 - 5;
        x1 = x0 + 10;
        SelectObject(memdc, grey);
        MoveToEx(memdc, x0, tr.bottom - 2, 0);
        LineTo(memdc, x1, tr.bottom - 2);
        MoveToEx(memdc, x0, tr.bottom - 1, 0);
        LineTo(memdc, x1, tr.bottom - 1);
        SelectObject(memdc, oldpen);
    }
    SelectObject(memdc, oldbmp);
    SelectObject(memdc, oldfont);
    DeleteObject(white);
    if (button->colour != -1)
        DeleteObject(face);
    DeleteObject(grey);
    DeleteDC(memdc);
    c = InitCtrlBlock(hwnd, obj);
    c->bmp0 = bmp_up;
    c->bmp1 = bmp_down;
    c->bmp2 = bmp_grey;
    c->kind = 6;
    SetWindowLong(hwnd, GWL_WNDPROC, (long)(void *)bitmap_button_proc);
    return hwnd;
}
