/* free_resource -- shipped at 1078:0186.
 *
 * Ported by tools/port.py from matched/platform/free_resource.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal FreeResource(col_i16 h);
#endif

col_i16 free_resource(void *p)
{
    return FreeResource((col_i16)p);
}
