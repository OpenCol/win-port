/* mci_can_open: candidate for 1078:2d51 (109 bytes).  Verdict: RELOCATABLE.
 *
 * Tests whether the MCI device type "avivideo" can be opened (MCI_OPEN by type): 1 when the open
 * succeeds, 0 otherwise. The device is not closed here.
 */

#include "colonization.h"

typedef struct { unsigned long dwCallback; unsigned wDeviceID; unsigned wReserved0; const char *lpstrDeviceType; const char *lpstrElementName; const char *lpstrAlias; unsigned long extra; unsigned extra2; unsigned extra3; } MCI_OPEN_PARMS_EX;

int mci_can_open(void)
{
    MCI_OPEN_PARMS_EX op;

    op.dwCallback = 0;
    op.wDeviceID = 0;
    op.lpstrDeviceType = "avivideo";
    op.lpstrElementName = 0;
    op.lpstrAlias = 0;
    op.extra = 0;
    op.extra2 = 0;
    return mciSendCommand(0, 0x803, 0x2000L, (unsigned long)(void *)&op) == 0;
}
