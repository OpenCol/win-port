/* MSHideCursor -- shipped at 1078:292f.
 *
 * Ported by tools/port.py from matched/platform/MSHideCursor.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ShowCursor(col_i16 show);
#endif

void MSHideCursor(void)
{
    while (ShowCursor(0) >= 0)
        ;
}
