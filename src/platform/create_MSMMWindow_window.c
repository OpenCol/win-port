/* create_MSMMWindow_window: candidate for 1078:0426 (145 bytes).  Verdict: RELOCATABLE.
 *
 * Registers the "MSMMWindow" class (procedure mci_notify_wndproc, no extras, no cursor or
 * brush) and creates its invisible default-placed window, kept at DGROUP:0x5726 as the
 * target of MCI notifications.
 */

#include "colonization.h"

typedef long (*WNDPROC)(unsigned, unsigned, unsigned, long);
typedef struct { unsigned style; WNDPROC lpfnWndProc; int cbClsExtra, cbWndExtra; unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName; } WNDCLASS;

extern unsigned RegisterClass(WNDCLASS *wc);

void create_MSMMWindow_window(void)
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = mci_notify_wndproc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hinstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = 0;
    wc.lpszMenuName = 0;
    wc.lpszClassName = "MSMMWindow";
    RegisterClass(&wc);
    g_cd_notify_hwnd = CreateWindow("MSMMWindow", 0, 0L, 0x8000, 0x8000, 0x8000, 0x8000, 0, 0, g_hinstance, 0);
}
