/* MSCloseCD -- shipped at 1078:096d.
 *
 * Ported by tools/port.py from matched/platform/MSCloseCD.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_cd_device;                         DGROUP:0x3c7a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 param);;
#endif

void MSCloseCD(void)
{
    if (g_cd_device != 0) {
        mciSendCommand(g_cd_device, 0x808, 0L, 0L);
        mciSendCommand(g_cd_device, 0x804, 0L, 0L);
        g_cd_device = 0;
    }
}
