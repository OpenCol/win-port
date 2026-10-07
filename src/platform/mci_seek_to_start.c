/* mci_seek_to_start: candidate for 1078:2fea (38 bytes).  Verdict: RELOCATABLE.
 *
 * MCI message 0x807 with flag 0x100 to the device whose id is at +4.
 *
 * RENAMED 2026-09-26 from mci_stop: Sends mciSendCommand(dev->id, 0x807, 0x100, 0): 0x807 is MCI_SEEK and 0x100 is MCI_SEEK_TO_START, so it rewinds the device to its start. MCI_STOP is 0x808, as its siblings stop_mci_device_1078_0940 and MSCloseCD send it. The device is never stopped, so the old name gets the operation wrong.
 */

#include "colonization.h"

#include "records.h"

void mci_seek_to_start(MciDev *device)
{
    mciSendCommand(device->id, 0x807, 0x100L, 0L);
}
