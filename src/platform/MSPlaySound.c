/* MSPlaySound -- shipped at 1078:053e.
 *
 * Ported by tools/port.py from matched/platform/MSPlaySound.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal sndPlaySound(const char *name, col_u16 flags);
#endif

col_i16 MSPlaySound(const char *name)
{
    return sndPlaySound(name, 1);
}
