/* MSCloseCD: candidate for 1078:096d (63 bytes).  Verdict: RELOCATABLE.
 *
 * MCI_STOP then MCI_CLOSE on the open CD device (id at DGROUP:0x3c7a), then forgets it.
 */

#include "colonization.h"

void MSCloseCD(void)
{
    if (g_cd_device != 0) {
        mciSendCommand(g_cd_device, 0x808, 0L, 0L);
        mciSendCommand(g_cd_device, 0x804, 0L, 0L);
        g_cd_device = 0;
    }
}
