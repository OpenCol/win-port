/* MSDestroyMenu -- shipped at 1070:d613.
 *
 * Ported by tools/port.py from matched/platform/MSDestroyMenu.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DestroyMenu(col_u16 hmenu);;
#endif

void MSDestroyMenu(col_u16 hmenu)
{
    if (hmenu != 0)
        DestroyMenu(hmenu);
}
