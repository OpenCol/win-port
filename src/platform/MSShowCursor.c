/* MSShowCursor -- shipped at 1078:291a.
 *
 * Ported by tools/port.py from matched/platform/MSShowCursor.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ShowCursor(col_i16 show);;
#endif

void MSShowCursor(void)
{
    ShowCursor(1);
}
