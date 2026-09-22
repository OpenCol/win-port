/* mci_can_open -- shipped at 1078:2d51.
 *
 * Ported by tools/port.py from matched/platform/mci_can_open.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
#endif

typedef struct { col_u32 dwCallback; col_u16 wDeviceID; col_u16 wReserved0; const char *lpstrDeviceType; const char *lpstrElementName; const char *lpstrAlias; col_u32 extra; col_u16 extra2; col_u16 extra3; } MCI_OPEN_PARMS_EX;


col_i16 mci_can_open(void)
{
    MCI_OPEN_PARMS_EX op;

    op.dwCallback = 0;
    op.wDeviceID = 0;
    op.lpstrDeviceType = "avivideo";
    op.lpstrElementName = 0;
    op.lpstrAlias = 0;
    op.extra = 0;
    op.extra2 = 0;
    return mciSendCommand(0, 0x803, 0x2000L, (col_u32)(void *)&op) == 0;
}
