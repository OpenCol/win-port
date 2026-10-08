/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* close_mci_device: candidate for 1078:2dbe (51 bytes).  Verdict: RELOCATABLE.
 *
 * Closes (MCI_CLOSE) the MCI device named "avivideo", looked up by name.
 */

#include "colonization.h"

void close_mci_device(void)
{
    unsigned dev;
    unsigned long parms;

    dev = mciGetDeviceID("avivideo");
    mciSendCommand(dev, 0x804, 0L, (uintptr_t)(void *)&parms);
}
