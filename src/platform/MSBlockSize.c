/* MSBlockSize -- shipped at 1070:d157.
 *
 * Ported by tools/port.py from matched/platform/MSBlockSize.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal GlobalSize(col_i16 h);;
#endif

col_u32 MSBlockSize(col_i16 h)
{
    return GlobalSize(h);
}
