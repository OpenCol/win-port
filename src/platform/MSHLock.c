/* MSHLock -- shipped at 1070:d073.
 *
 * Ported by tools/port.py from matched/platform/MSHLock.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void * pascal GlobalLock(col_i16 h);
#endif

void * MSHLock(col_i16 h)
{
    if (h != 0)
        return GlobalLock(h);
    else
        return 0;
}
