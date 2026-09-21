/* MSGetTicks -- shipped at 1078:181a.
 *
 * Ported by tools/port.py from matched/platform/MSGetTicks.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal GetTickCount(void);
#endif

col_i32 MSGetTicks(void)
{
    return GetTickCount() * 6L / 100;
}
