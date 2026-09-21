/* free_space -- shipped at 1070:d214.
 *
 * Ported by tools/port.py from matched/platform/free_space.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u32 pascal GetFreeSpace(col_u16 flags);
#endif

col_u32 free_space(void)
{
    return GetFreeSpace(0);
}
