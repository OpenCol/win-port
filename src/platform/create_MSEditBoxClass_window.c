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
typedef struct { short hfont, height; } Font;
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class SScrollBar : public DlgControl { public: long on_change; long on_track; short page; short f24; short pos; SScrollBar(); ~SScrollBar(); };
class SCheckBox : public DlgControl { public: long on_change; short checked; short enabled; SCheckBox(); ~SCheckBox(); };
class SRadioSet : public DlgControl { public: long on_change; long on_dblclk; long on_rbutton_down; short count; short cols; short cur_item; short hmem; void *items; SRadioSet(); ~SRadioSet(); };
class SEditBox : public DlgControl { public: long on_change; long key_filter; char multiline; char pad_23; long f24; SEditBox(); ~SEditBox(); };
class SListBox : public DlgControl { public: short count; long f1c; SListBox(); ~SListBox(); };
#include "records.h"
typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);
typedef struct { unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra; unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName; } WNDCLASS;

extern "C" unsigned RegisterClass(WNDCLASS *wc);

extern "C" void *ctl_window_1070_96a8(void *ctl);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" MSWin *get_win_1070_95f0(void *o);
extern "C" unsigned font_handle_1070_9806(void *ref);

extern "C" long editbox_subclass_proc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam);

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
    SetWindowLong(hwnd, -4, (long)(void *)editbox_subclass_proc);
    SetWindowLong(hwnd, g_editbox_extra, (long)g_editbox_proc);
    SetWindowLong(hwnd, g_editbox_extra + 4, (long)edit);
    return hwnd;
}
