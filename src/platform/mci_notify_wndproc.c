/* mci_notify_wndproc -- shipped at 1078:04b7.
 *
 * Ported by tools/port.py from matched/platform/mci_notify_wndproc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_mci_device;                        DGROUP:0x3c78
 *   extern unsigned g_cd_device;                         DGROUP:0x3c7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i32 pascal DefWindowProc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp);
extern col_u16 pascal mciSendCommand(col_u16 id, col_u16 msg, col_i32 p1, col_i32 p2);
#endif

#include "w16const.h"


extern void call_midi_done_callback(void);
extern void call_cd_done_callback(void);



col_i32 pascal mci_notify_wndproc(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp)
{
    switch (msg) {
    case MM_MCINOTIFY:
        if (wp == 1) {
            mciSendCommand((col_u16)lp, 0x804, 0, 0);
            if ((col_u16)lp == g_mci_device) {
                call_midi_done_callback();
                g_mci_device = 0;
            } else if ((col_u16)lp == g_cd_device) {
                call_cd_done_callback();
                g_cd_device = 0;
            }
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
