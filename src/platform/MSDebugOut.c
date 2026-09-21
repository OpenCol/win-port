/* MSDebugOut -- shipped at 1070:848a.
 *
 * Ported by tools/port.py from matched/platform/MSDebugOut.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OutputDebugString(const char *s);
#endif

col_i16 MSDebugOut(const char *s)
{
    OutputDebugString(s);
    OutputDebugString("\n");
    return 1;
}
