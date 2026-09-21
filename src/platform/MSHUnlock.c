/* MSHUnlock -- shipped at 1070:d0a0.
 *
 * Ported by tools/port.py from matched/platform/MSHUnlock.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GlobalUnlock(col_i16 h);;
#endif

col_i32 MSHUnlock(col_i16 h)
{
    if (h != 0)
        GlobalUnlock(h);
    return 0L;
}
