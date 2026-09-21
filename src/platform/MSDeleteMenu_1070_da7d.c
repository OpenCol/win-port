/* MSDeleteMenu_1070_da7d -- shipped at 1070:da7d.
 *
 * Ported by tools/port.py from matched/platform/MSDeleteMenu_1070_da7d.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteMenu(col_u16 hmenu, col_u16 item, col_u16 flags);;
#endif

void MSDeleteMenu_1070_da7d(col_u16 hmenu, col_u16 id)
{
    if (hmenu != 0)
        DeleteMenu(hmenu, id, 0);
}
