/* lock_resource -- shipped at 1078:0386.
 *
 * Ported by tools/port.py from matched/platform/lock_resource.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void * pascal LockResource(col_i16 h);
#endif

void * lock_resource(void *p)
{
    return LockResource((col_i16)p);
}
