/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* mci_close_device: candidate for 1078:2df1 (43 bytes).  Verdict: RELOCATABLE.
 *
 * MCI message 0x804 with a four-byte parameter block on the stack to the
 * device whose id is at +4.
 *
 * RENAMED 2026-09-26 from mci_status: Sends MCI message 0x804 to the device id at +4. 0x804 is MCI_CLOSE; MCI_STATUS is 0x814, which cd_next_track uses in the same module. The body closes the device and queries nothing.
 */

#include "colonization.h"

#include "records.h"

void mci_close_device(MciDev *device)
{
    MciParms p;

    mciSendCommand(device->id, 0x804, 0L, (uintptr_t)(void *)&p);
}
