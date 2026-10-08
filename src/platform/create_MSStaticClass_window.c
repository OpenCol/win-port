/* PORT: SetWindowLong's value is intptr_t, not long: the game stores pointers there, 8 bytes on 64-bit Windows (W9). */
/* PORT: this file's own declarations of ctl_window_1070_96a8 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of get_win_1070_95f0 removed: the header declares it from the definition (W1 step 3). */
/* create_MSStaticClass_window: candidate for 1070:60ae (399 bytes).  Verdict: RELOCATABLE.
 *
 * Creates a static-text control. The first call registers "MSStaticClass" (guard at
 * DGROUP:0x33d0): a throw-away "STATIC" window yields Windows' static procedure, kept
 * at DGROUP:0x56da, and a WNDCLASS with 10 extra bytes is registered (the LoadCursor result
 * is thrown away, as shipped). Then the control window is created with the text, its
 * procedure replaced by static_text_wndproc and `param` stored in window long 6.
 *
 * RENAMED 2026-09-26 from register_MSStaticClass: Creates a static-text control window with the given text and rectangle, subclasses it with text_out, stores `param` in window long 6, and returns the HWND. It registers the MSStaticClass window class only on the first call, guarded by DGROUP:0x33d0. The old name describes that one-time setup rather than the per-call job and return value. Its sibling register_MSEditBoxClass (1070:5550, not in this chunk) has the same pattern and the same naming problem. It creates the control's window on every call and returns its HWND; registering the class happens only on the first call. Named like its siblings create_MSMovieClass_window and create_MSWindowClass_window.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);
typedef struct { unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra; unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName; } WNDCLASS;

extern unsigned RegisterClass(WNDCLASS *wc);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern long static_text_wndproc(unsigned hwnd, unsigned msg, unsigned wparam, long lparam);

unsigned create_MSStaticClass_window(RECT *r, void *ctl, const char *text, long param)
{
    WNDCLASS wc;
    unsigned hwnd;

    if (g_static_registered == 0) {
        hwnd = CreateWindow("STATIC", "", 0x50000000L, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                            get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
        g_static_proc = (WNDPROC)GetWindowLong(hwnd, -4);
        wc.style = 0x80;
        wc.lpfnWndProc = g_static_proc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 10;
        wc.hInstance = g_hinstance;
        wc.hIcon = 0;
        wc.hCursor = 0;
        LoadCursor(0, (const char *)0x7f00L);
        wc.hbrBackground = 0;
        wc.lpszMenuName = 0;
        wc.lpszClassName = "MSStaticClass";
        RegisterClass(&wc);
        g_static_registered = 1;
        DestroyWindow(hwnd);
    }
    hwnd = CreateWindow("MSStaticClass", text, 0x58000000L, r->left, r->top, rect_width_1070_962a(r), rect_height_1070_9649(r),
                        get_win_1070_95f0((Screen *)(ctl_window_1070_96a8((DlgControl *)ctl)))->hwnd, 0, g_hinstance, 0);
    SetWindowLong(hwnd, -4, (intptr_t)(void *)static_text_wndproc);
    SetWindowLong(hwnd, 6, param);
    return hwnd;
}
