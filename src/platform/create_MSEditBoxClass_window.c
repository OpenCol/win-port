/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* PORT: this file's own declarations of ctl_window_1070_96a8, font_handle_1070_9806 removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* create_MSEditBoxClass_window: candidate for 1070:5550 (638 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: create_MSEditBoxClass_window
 *
 * Creates an edit control. Style 0x40800000 plus WS_VISIBLE (0x10000000) for `visible` and, from the
 * flag bits: 1 ES_AUTOHSCROLL, 2 ES_MULTILINE (and the owner's byte +0x22 marks it
 * multi-line), 4 ES_PASSWORD|ES_AUTOHSCROLL, 8 ES_UPPERCASE, 0x10 ES_LOWERCASE, 0x20
 * WS_VSCROLL|ES_AUTOVSCROLL, 0x40 WS_HSCROLL|ES_AUTOHSCROLL. The first call registers
 * "MSEditBoxClass" (guard DGROUP:0x33cc) from a throw-away "EDIT" window, keeping its
 * procedure (0x56ce) and window extra bytes (0x56d2). The control gets the default font
 * (WM_SETFONT), the subclass procedure, and the old procedure and owner in the extra longs.
 *
 * RENAMED 2026-09-26 from register_MSEditBoxClass: Creates and returns an edit control window. It builds the style from the flag bits, calls CreateWindow on MSEditBoxClass, sets the default font and installs editbox_subclass_proc. The class is registered only on the first call, behind a guard. The name gives that minor step instead of the job, and edit_box_init stores the returned HWND. Mac counterpart MSInitEditBox(Rect*, void*, unsigned long). It creates the control's window on every call and returns its HWND; registering the class happens only on the first call. Named like its siblings create_MSMovieClass_window and create_MSWindowClass_window.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
/* PORT: this file's own Font, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SButton, SImageButton, SRegion, SScrollBar, SCheckBox, SRadioSet, SEditBox, SListBox removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-


class DlgControl;











#include "records.h"
typedef long (*WNDPROC)(unsigned, unsigned, unsigned, intptr_t);
typedef struct { unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra; unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName; } WNDCLASS;

extern "C" unsigned RegisterClass(WNDCLASS *wc);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" long editbox_subclass_proc(unsigned hwnd, unsigned msg, unsigned wparam, intptr_t lparam);

unsigned create_MSEditBoxClass_window(RECT *r, SEditBox *edit, unsigned long flags, int visible)
{
    FontRec *font;
    unsigned long style;
    WNDCLASS wc;
    unsigned hwnd;

    style = 0x40800000L;
    if (visible)
        style |= WS_VISIBLE;
    if (flags & 1L)
        style |= 0x80L;
    if (flags & 2L) {
        style |= 4L;
        edit->multiline = 1;
    }
    if (flags & 4L)
        style |= 0xa0L;
    if (flags & 8L)
        style |= 8L;
    if (flags & 0x10L)
        style |= 0x10L;
    if (flags & 0x20L)
        style |= 0x200040L;
    if (flags & 0x40L)
        style |= 0x100080L;
    if (g_editbox_registered == 0) {
        hwnd = CreateWindow("EDIT", "", style, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                            get_win_1070_95f0((Screen *)(ctl_window_1070_96a8(edit)))->hwnd, 0, g_hinstance, 0);
        g_editbox_proc = (WNDPROC)GetWindowLong(hwnd, -4);
        wc.style = 0x88;
        wc.lpfnWndProc = g_editbox_proc;
        wc.cbClsExtra = 0;
        g_editbox_extra = GetClassWord(hwnd, -18);
        wc.cbWndExtra = g_editbox_extra + 8;
        wc.hInstance = g_hinstance;
        wc.hIcon = 0;
        wc.hCursor = LoadCursor(0, (const char *)0x7f01L);
        wc.hbrBackground = 0;
        wc.lpszMenuName = 0;
        wc.lpszClassName = "MSEditBoxClass";
        RegisterClass(&wc);
        g_editbox_registered = 1;
        DestroyWindow(hwnd);
    }
    hwnd = CreateWindow("MSEditBoxClass", "", 0x40800000L | style, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8(edit)))->hwnd, 0, g_hinstance, 0);
    font = (FontRec *)MSHLock(font_handle_1070_9806((Font *)g_editbox_font));
    SendMessage(hwnd, 0x30, font->hfont, 0L);
    MSHUnlock(font_handle_1070_9806((Font *)g_editbox_font));
    SetWindowLong(hwnd, -4, (intptr_t)(void *)editbox_subclass_proc);
    SetWindowLong(hwnd, g_editbox_extra, (intptr_t)g_editbox_proc);
    SetWindowLong(hwnd, g_editbox_extra + 4, (intptr_t)edit);
    return hwnd;
}
