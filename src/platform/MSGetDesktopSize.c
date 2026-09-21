/* MSGetDesktopSize -- shipped at 1078:2566.
 *
 * Ported by tools/port.py from matched/platform/MSGetDesktopSize.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetSystemMetrics(col_i16 index);;
extern void pascal SetRect(void *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);;
#endif

void MSGetDesktopSize(void *r)
{
    SetRect(r, 0, 0, GetSystemMetrics(0), GetSystemMetrics(1));
}
