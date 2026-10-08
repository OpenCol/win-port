/* PORT: mciSendCommand's parameter block is passed as uintptr_t, not unsigned long: 8 bytes on 64-bit Windows (W9). */
/* mci_get_rect: candidate for 1078:2fac (62 bytes).  Verdict: RELOCATABLE.
 *
 * MCI message 0x843 (MCI_WHERE) with flag 0x20000 to the device at +4;
 * the rectangle it returns is copied to *r.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
typedef struct { unsigned long dwCallback; RECT rc; } MciRectParms;

void mci_get_rect(MciDev *device, RECT *r)
{
    MciRectParms p;

    mciSendCommand(device->id, 0x843, 0x20000L, (uintptr_t)(void *)&p);
    CopyRect(r, &p.rc);
}
