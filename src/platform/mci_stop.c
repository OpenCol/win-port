/* mci_stop -- shipped at 1078:2fea.
 *
 * Ported by tools/port.py from matched/platform/mci_stop.c.
 *
 * The file's own MciDev dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);;
#endif

void mci_stop(MciDev *d)
{
    mciSendCommand(d->id, 0x807, 0x100L, 0L);
}
