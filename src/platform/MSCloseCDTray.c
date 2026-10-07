/* MSCloseCDTray: candidate for 1078:0b9c (212 bytes).  Verdict: RELOCATABLE.
 *
 * Closes the CD door. With neither a playing device nor a tray device known, "cdaudio" is
 * opened, given MCI_SET flag 0x100 (as shipped -- the open-door flag) and closed again;
 * with a tray device known (from MSOpenCDTray), that id is forgotten, the playing device
 * closed, and the tray device sent MCI_SET_DOOR_CLOSED and MCI_CLOSE.
 */

#include "colonization.h"

#include "w16mci.h"
   /* 16 bytes in this build */

/* same object: 4-byte `push cs; call ` (rule 47) */

void MSCloseCDTray(void)
{
    unsigned long flags;
    unsigned long r;
    MCI_SET_PARMS sp;
    MCI_OPEN_PARMS op;
    unsigned dev;

    if (g_cd_device == 0 && g_cd_tray_device == 0) {
        op.lpstrDeviceType = "cdaudio";
        if ((r = mciSendCommand(0, 0x803, 0x2000L, (unsigned long)(void *)&op)) != 0)
            return;
        dev = op.wDeviceID;
        flags = 0x100L;
        r = mciSendCommand(dev, 0x80d, flags, (unsigned long)(void *)&sp);
        mciSendCommand(dev, 0x804, 0L, 0L);
    } else if (g_cd_tray_device != 0) {
        dev = g_cd_tray_device;
        g_cd_tray_device = 0;
        MSCloseCD();
        flags = 0x200L;
        r = mciSendCommand(dev, 0x80d, flags, (unsigned long)(void *)&sp);
        mciSendCommand(dev, 0x804, 0L, 0L);
    }
}
