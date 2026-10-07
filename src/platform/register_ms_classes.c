/* register_ms_classes: candidate for 1070:b16f (319 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `unregister_window_classes`'s opposite number, 0x13f bytes above it: one WNDCLASS on the
 * stack, filled and re-filled four times, and `RegisterClass` after each. The name
 * `platform_startup` was already taken by 1070:b0f9, the 40-byte hub that calls this
 * one along with three others, so this file carries its address. The four are
 * "MSWindowClass" with `main_window_message_handler`, "MSMovieClass" with `mci_message_handler`,
 * "MSControlClass" with `control_message_handler`, and "MSMrTimerClass" with
 * `timer_window_proc` -- the whole of this build's window-class table, in one function.
 *
 * The extra bytes per window are what each class is for: 0x14 for the two window classes,
 * 0x10 for controls, and 4 for the timer, which needs only a handle.
 *
 * Only the control class takes a cursor, `LoadCursor(0, IDC_ARROW)`; the others leave it zero
 * and paint their own. The `PUSH DWORD 0x7f00` is `MAKEINTRESOURCE(32512)` already folded to
 * a constant pointer, which is the same folding rule 57 is about, seen where it does no
 * harm.
 *
 * Every field is re-set between registrations, even those whose value does not change, so
 * the four blocks are identical in shape and only the values differ.
 *
 * THE TWO LISTS DO NOT MATCH. `unregister_window_classes` names four classes, and they are
 * not these four: it drops "MSAppWindow", which nothing here registers, and never drops
 * "MSMovieClass", which this function does register. So one class is unregistered that was
 * never created and one is left registered at exit.
 */

#include "colonization.h"

typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);
typedef struct { unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra; unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName; } WNDCLASS;

extern unsigned RegisterClass(WNDCLASS *wc);

void register_ms_classes(void)
{
    WNDCLASS wc;

    wc.style = 0x28;
    wc.lpfnWndProc = (WNDPROC)main_window_message_handler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0x14;
    wc.hInstance = g_hinstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0L;
    wc.lpszClassName = "MSWindowClass";
    RegisterClass(&wc);

    wc.style = 0x28;
    wc.lpfnWndProc = (WNDPROC)mci_message_handler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0x14;
    wc.hInstance = g_hinstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0L;
    wc.lpszClassName = "MSMovieClass";
    RegisterClass(&wc);

    wc.style = 0x88;
    wc.lpfnWndProc = (WNDPROC)control_message_handler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0x10;
    wc.hInstance = g_hinstance;
    wc.hIcon = 0;
    wc.hCursor = LoadCursor(0, (const char *)0x7f00L);
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0L;
    wc.lpszClassName = "MSControlClass";
    RegisterClass(&wc);

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC)timer_window_proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 4;
    wc.hInstance = g_hinstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0L;
    wc.lpszClassName = "MSMrTimerClass";
    RegisterClass(&wc);
}
