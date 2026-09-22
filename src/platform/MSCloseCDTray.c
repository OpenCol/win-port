/* MSCloseCDTray -- shipped at 1078:0b9c.
 *
 * Ported by tools/port.py from matched/platform/MSCloseCDTray.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_cd_device;                         DGROUP:0x3c7a
 *   extern unsigned g_cd_tray_device;                    DGROUP:0x3c7c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSCloseCD(void);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

#include "w16mci.h"
   /* 16 bytes in this build */



/* same object: 4-byte `push cs; call ` (rule 47) */

void MSCloseCDTray(void)
{
    col_u32 flags;
    col_u32 r;
    MCI_SET_PARMS sp;
    MCI_OPEN_PARMS op;
    col_u16 dev;

    if (g_cd_device == 0 && g_cd_tray_device == 0) {
        op.lpstrDeviceType = "cdaudio";
        if ((r = mciSendCommand(0, 0x803, 0x2000L, (col_u32)(void *)&op)) != 0)
            return;
        dev = op.wDeviceID;
        flags = 0x100L;
        r = mciSendCommand(dev, 0x80d, flags, (col_u32)(void *)&sp);
        mciSendCommand(dev, 0x804, 0L, 0L);
    } else if (g_cd_tray_device != 0) {
        dev = g_cd_tray_device;
        g_cd_tray_device = 0;
        MSCloseCD();
        flags = 0x200L;
        r = mciSendCommand(dev, 0x80d, flags, (col_u32)(void *)&sp);
        mciSendCommand(dev, 0x804, 0L, 0L);
    }
}
