/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* MSOpenCDTray: candidate for 1078:0af3 (169 bytes).  Verdict: RELOCATABLE.
 *
 * Opens the CD door: with no device open, MCI_OPEN "cdaudio" (returning on failure) and
 * MCI_SET with MCI_SET_DOOR_OPEN on the new device; otherwise the current device is closed
 * first and the same MCI_SET sent to its (already cleared) id. The device id used is kept
 * at DGROUP:0x3c7c.
 */

#include "colonization.h"

#include "w16mci.h"
   /* 16 bytes in this build */

/* same object: 4-byte `push cs; call ` (rule 47) */

void MSOpenCDTray(void)
{
    unsigned long flags;
    unsigned long r;
    MCI_SET_PARMS sp;
    MCI_OPEN_PARMS op;
    unsigned dev;

    if (g_cd_device == 0) {
        op.lpstrDeviceType = "cdaudio";
        if ((r = mciSendCommand(0, 0x803, 0x2000L, (uintptr_t)(void *)&op)) != 0)
            return;
        dev = op.wDeviceID;
        flags = 0x100L;
        r = mciSendCommand(dev, 0x80d, flags, (uintptr_t)(void *)&sp);
    } else {
        dev = g_cd_device;
        MSCloseCD();
        flags = 0x100L;
        r = mciSendCommand(g_cd_device, 0x80d, flags, (uintptr_t)(void *)&sp);
    }
    g_cd_tray_device = dev;
}
