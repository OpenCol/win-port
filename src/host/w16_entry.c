/* The Windows build's start: the game on a thread of its own with the 64 MB stack it needs.
 *
 * The game needs a deep stack: its functions keep their records and buffers in locals. The
 * build used to link a 64 MB one (-Wl,--stack), but the executable's stack size is every
 * thread's default, not the main thread's alone: each thread the shell's open/save dialog
 * starts reserved 64 MB too, and a 32-bit process has 2 GB -- about 29 such threads (measured
 * under Wine). A tester's 32-bit Save button did nothing on Windows 7 while the 64-bit
 * program's worked: the dialog's own threads could not be made. So the executable keeps the
 * default stack, and the link's entry point (tools/undefined.py: -e) is port_entry: the C
 * runtime's start, and with it the game, runs on a thread with the 64 MB, as the Linux build's
 * WinMain does (w16_winelib.c). Its windows and message queue are that thread's; the first
 * thread waits for it.
 */
#ifndef __WINE__
#include "w16.h"

int WinMainCRTStartup(void);             /* mingw-w64's start (crtexe.c): constructors, then WinMain */

static DWORD WINAPI crt_thread(void *unused)
{
    (void)unused;
    return (DWORD)WinMainCRTStartup();   /* ends with exit(), which ends the process */
}

void port_entry(void)
{
    DWORD code = 0;
    HANDLE t = CreateThread(0, 64u << 20, crt_thread, 0, STACK_SIZE_PARAM_IS_A_RESERVATION, 0);
    if (!t)
        ExitProcess((UINT)WinMainCRTStartup());
    WaitForSingleObject(t, INFINITE);
    GetExitCodeThread(t, &code);
    ExitProcess(code);
}
#endif
