/* stop_mci_device_1078_0940 -- shipped at 1078:0940.
 *
 * Ported by tools/port.py from matched/platform/stop_mci_device_1078_0940.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_mci_device;                        DGROUP:0x3c78
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

void stop_mci_device_1078_0940(void)
{
    if (g_mci_device != 0) {
        mciSendCommand(g_mci_device, 0x808, 0L, 0L);
        g_mci_device = 0;
    }
}
