/* destroy_MSMMWindow_window -- shipped at 1078:09ac.
 *
 * Ported by tools/port.py from matched/platform/destroy_MSMMWindow_window.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_cd_notify_hwnd;                    DGROUP:0x5726
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSCloseCD(void);
void MSCloseSound(void);
void stop_mci_device_1078_0940(void);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal DestroyWindow(col_u16 hwnd);
#endif

/* same object: 4-byte `push cs; call ` */




void destroy_MSMMWindow_window(void)
{
    MSCloseSound();
    MSCloseCD();
    stop_mci_device_1078_0940();
    DestroyWindow(g_cd_notify_hwnd);
}
