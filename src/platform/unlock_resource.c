/* unlock_resource -- shipped at 1078:03a0.
 *
 * Ported by tools/port.py from matched/platform/unlock_resource.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GlobalUnlock(col_i16 h);
#endif

col_i16 unlock_resource(void *p)
{
    return GlobalUnlock((col_i16)p);
}
