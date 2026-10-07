/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* create_MSListBoxClass_window: candidate for 1070:5136 (472 bytes).  Verdict: RELOCATABLE.
 *
 * Creates a list-box control. The first call registers "MSListBoxClass" (guard at
 * DGROUP:0x33ca) from a throw-away "LISTBOX" window: its procedure (kept at 0x56c8) and
 * class extra bytes (0x56cc) are copied into a WNDCLASS with 8 more extra bytes. Then the
 * control window is created (style 0x40a10001, plus WS_VISIBLE (0x10000000) for `visible`), the subclass
 * procedure installed, and the old procedure and the region pointer stored in the two extra
 * window longs.
 *
 * RENAMED 2026-09-26 from register_MSListBoxClass: Creates and returns a list-box control window: CreateWindow of MSListBoxClass, installs listbox_subclass_proc, and stores the old proc and the owner in the extra longs. Registering the class is a one-time guarded step on the first call only. The name gives that minor step instead of the job, and the caller (listbox_init) stores the returned HWND. Mac counterpart MSInitListBox(Rect*, void*). It creates the control's window on every call and returns its HWND; registering the class happens only on the first call. Named like its siblings create_MSMovieClass_window and create_MSWindowClass_window.
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
extern long listbox_subclass_proc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam);

unsigned create_MSListBoxClass_window(RECT *r, void *ctl, int visible)
{
    unsigned long style;
    WNDCLASS wc;
    unsigned hwnd;

    style = 0x40a10000L;
    if (visible)
        style |= WS_VISIBLE;
    if (g_listbox_registered == 0) {
        hwnd = CreateWindow("LISTBOX", "", style, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                            get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
        g_listbox_proc = (WNDPROC)GetWindowLong(hwnd, -4);
        wc.style = 0x88;
        wc.lpfnWndProc = g_listbox_proc;
        wc.cbClsExtra = 0;
        g_listbox_extra = GetClassWord(hwnd, -18);
        wc.cbWndExtra = g_listbox_extra + 8;
        wc.hInstance = g_hinstance;
        wc.hIcon = 0;
        wc.hCursor = LoadCursor(0, (const char *)0x7f00L);
        wc.hbrBackground = 0;
        wc.lpszMenuName = 0;
        wc.lpszClassName = "MSListBoxClass";
        RegisterClass(&wc);
        g_listbox_registered = 1;
        DestroyWindow(hwnd);
    }
    style = 0x40a10001L;
    if (visible)
        style |= WS_VISIBLE;
    hwnd = CreateWindow("MSListBoxClass", "", style, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
    SetWindowLong(hwnd, -4, (long)(void *)listbox_subclass_proc);
    SetWindowLong(hwnd, g_listbox_extra, (long)g_listbox_proc);
    SetWindowLong(hwnd, g_listbox_extra + 4, (long)ctl);
    return hwnd;
}
