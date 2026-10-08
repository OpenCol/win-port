/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* MSUnPauseCD: candidate for 1078:0d1d (183 bytes).  Verdict: RELOCATABLE.
 *
 * Queries the CD mode; a stopped device is started (MCI_PLAY), a paused one resumed
 * (MCI_RESUME).
 */

#include "colonization.h"

#include "w16mci.h"
   /* 16 bytes in this build */

void MSUnPauseCD(void)
{
    MCI_GENERIC_PARMS gp;
    unsigned long flags;
    unsigned long r;
    MCI_STATUS_PARMS st;
    MCI_PLAY_PARMS pp;

    flags = 0x100L;
    st.dwItem = 4L;
    r = mciSendCommand(g_cd_device, 0x814, flags, (uintptr_t)(void *)&st);
    if (r != 0)
        return;
    if (st.dwReturn == 0x20dL) {
        flags = 0;
        r = mciSendCommand(g_cd_device, 0x806, flags, (uintptr_t)(void *)&pp);
        if (r != 0)
            return;
    } else if (st.dwReturn == 0x211L) {
        flags = 0;
        r = mciSendCommand(g_cd_device, 0x855, flags, (uintptr_t)(void *)&gp);
    }
}
