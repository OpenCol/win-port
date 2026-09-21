/* MSGetSubMenu -- shipped at 1070:d9e3.
 *
 * Ported by tools/port.py from matched/platform/MSGetSubMenu.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);;
#endif

col_u16 MSGetSubMenu(col_u16 hmenu, col_i16 pos)
{
    if (hmenu != 0)
        return GetSubMenu(hmenu, pos - 1);
    else
        return 0;
}
