/* MSPauseCD: candidate for 1078:0c70 (173 bytes).  Verdict: RELOCATABLE.
 *
 * Queries the CD mode (MCI_STATUS_MODE); a stopped device is started (MCI_PLAY), anything
 * else paused (MCI_PAUSE). Each failed command returns early.
 */

#include "colonization.h"

#include "w16mci.h"
   /* 16 bytes in this build */

void MSPauseCD(void)
{
    MCI_GENERIC_PARMS gp;
    unsigned long flags;
    unsigned long r;
    MCI_STATUS_PARMS st;
    MCI_PLAY_PARMS pp;

    flags = 0x100L;
    st.dwItem = 4L;
    r = mciSendCommand(g_cd_device, 0x814, flags, (unsigned long)(void *)&st);
    if (r != 0)
        return;
    if (st.dwReturn == 0x20dL) {
        flags = 0;
        r = mciSendCommand(g_cd_device, 0x806, flags, (unsigned long)(void *)&pp);
        if (r != 0)
            return;
    } else {
        flags = 0;
        r = mciSendCommand(g_cd_device, 0x809, flags, (unsigned long)(void *)&gp);
    }
}
