/* stop_mci_device_1078_0940: candidate for 1078:0940 (45 bytes).  Verdict: RELOCATABLE.
 *
 * Stops (MCI_STOP) the MCI device whose id is kept at DGROUP:0x3c78, if any, and forgets it.
 */

#include "colonization.h"

void stop_mci_device_1078_0940(void)
{
    if (g_mci_device != 0) {
        mciSendCommand(g_mci_device, 0x808, 0L, 0L);
        g_mci_device = 0;
    }
}
