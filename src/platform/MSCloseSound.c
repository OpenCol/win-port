/* MSCloseSound -- shipped at 1078:0559.
 *
 * Ported by tools/port.py from matched/platform/MSCloseSound.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal sndPlaySound(const char *name, col_u16 flags);;
#endif

col_i16 MSCloseSound(void)
{
    return sndPlaySound(0, 1);
}
