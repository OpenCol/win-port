/* close_mci_device -- shipped at 1078:2dbe.
 *
 * Ported by tools/port.py from matched/platform/close_mci_device.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal mciGetDeviceID(const char *name);
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

void close_mci_device(void)
{
    col_u16 dev;
    col_u32 parms;

    dev = mciGetDeviceID("avivideo");
    mciSendCommand(dev, 0x804, 0L, (col_u32)(void *)&parms);
}
