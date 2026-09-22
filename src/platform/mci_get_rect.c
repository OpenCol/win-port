/* mci_get_rect -- shipped at 1078:2fac.
 *
 * Ported by tools/port.py from matched/platform/mci_get_rect.c.
 *
 * The file's own MciDev dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal mciSendCommand(col_u16 dev, col_u16 msg, col_u32 flags, col_u32 parms);
extern void pascal CopyRect(RECT *dst, RECT *src);
#endif

typedef struct { col_u32 dwCallback; RECT rc; } MciRectParms;



void mci_get_rect(MciDev *d, RECT *r)
{
    MciRectParms p;

    mciSendCommand(d->id, 0x843, 0x20000L, (col_u32)(void *)&p);
    CopyRect(r, &p.rc);
}
