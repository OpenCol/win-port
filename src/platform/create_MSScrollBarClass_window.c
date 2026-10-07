/* create_MSScrollBarClass_window: candidate for 1070:5b29 (507 bytes).  Verdict: RELOCATABLE.
 *
 * Creates a scroll-bar control. The first call registers "MSScrollBarClass" (guard at
 * DGROUP:0x56d6) from a throw-away "SCROLLBAR" window whose procedure and window
 * extra-byte count (GCW_CBWNDEXTRA, kept at 0x56d8) go into a WNDCLASS of fixed style 0x8b with 6 more
 * extra bytes and a grey background brush; the throw-away window's GWL_STYLE replaces the computed style
 * for this first control.
 * Then the control window is created (style 0x40000000, bit 0 set when `style_in` is non-zero, plus
 * WS_VISIBLE (0x10000000) for `visible`), the region pointer stored in the window long at the offset kept at 0x56d8, the first of the 6 extra bytes past the SCROLLBAR class's own, the
 * bar shown and its range set to 1..100.
 *
 * RENAMED 2026-09-26 from register_MSScrollBarClass: The function's job is to create a scroll-bar control and return its HWND. scrollbar_init stores the result in o->hwnd. It CreateWindows an MSScrollBarClass window, stores the region pointer in a window long, shows the bar and sets its range to 1..100. Registering the class only happens on the first call, behind the guard at DGROUP:0x56d6, so the name describes a one-time side effect and not the function's work. The siblings register_MSEditBoxClass, register_MSListBoxClass and register_MSStaticClass (not in this chunk) follow the same create-and-return-HWND pattern and have the same naming problem. The create_MSMovieClass_window form is the corpus's existing convention for this. It creates the control's window on every call and returns its HWND; registering the class happens only on the first call. Named like its siblings create_MSMovieClass_window and create_MSWindowClass_window.
 */

#include "colonization.h"

#include "w16const.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);
typedef struct { unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra; unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName; } WNDCLASS;

extern unsigned RegisterClass(WNDCLASS *wc);

extern void *ctl_window_1070_96a8(void *ctl);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern MSWin *get_win_1070_95f0(void *o);

unsigned create_MSScrollBarClass_window(RECT *r, void *ctl, unsigned long style_in, int visible)
{
    unsigned long style;
    WNDPROC proc;
    WNDCLASS wc;
    unsigned hwnd;

    style = 0x40000000L;
    if (visible)
        style |= WS_VISIBLE;
    if (style_in != 0)
        style |= 1;
    else
        style |= 0;
    if (visible)
        style_in |= WS_VISIBLE;
    if (g_scrollbar_registered == 0) {
        hwnd = CreateWindow("SCROLLBAR", "", 0x40000000L | style_in, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                            get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
        proc = (WNDPROC)GetWindowLong(hwnd, -4);
        style = GetWindowLong(hwnd, -16);
        wc.style = 0x8b;
        wc.lpfnWndProc = proc;
        wc.cbClsExtra = 0;
        g_scrollbar_extra = GetClassWord(hwnd, -18);
        wc.cbWndExtra = g_scrollbar_extra + 6;
        wc.hInstance = g_hinstance;
        wc.hIcon = 0;
        wc.hCursor = LoadCursor(0, (const char *)0x7f00L);
        wc.hbrBackground = GetStockObject(1);
        wc.lpszMenuName = 0;
        wc.lpszClassName = "MSScrollBarClass";
        RegisterClass(&wc);
        g_scrollbar_registered = 1;
        DestroyWindow(hwnd);
    }
    hwnd = CreateWindow("MSScrollBarClass", "", style, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
    SetWindowLong(hwnd, g_scrollbar_extra, (long)ctl);
    ShowScrollBar(hwnd, 2, 1);
    scroll_set_range(hwnd, 1, 100);
    return hwnd;
}
