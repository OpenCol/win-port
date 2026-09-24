/* MSReleaseFont -- shipped at 1078:134c.
 *
 * Ported by tools/port.py from matched/platform/MSReleaseFont.c.
 *
 * The file's own FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteObject(col_u16 hobj);
#endif

extern FontRec * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_u16 h);


void MSReleaseFont(col_u16 h)
{
    FontRec *f = MSHLock(h);

    if (f->owned == 1)
        DeleteObject(f->hfont);
    MSHUnlock(h);
    MSDisposeHandle(h);
}
