/* MSAppendMenu -- shipped at 1070:db50.
 *
 * Ported by tools/port.py from matched/platform/MSAppendMenu.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal AppendMenu(col_u16 hmenu, col_u16 flags, col_u16 id, char *s);;
extern col_i16 pascal GetMenuItemCount(col_u16 hmenu);;
#endif

void MSAppendMenu(col_u16 hmenu, char *text, col_u16 id)
{
    col_i16 n;
    col_u16 flags;

    if (hmenu != 0) {
        n = GetMenuItemCount(hmenu);
        if (n % 22 == 0 && n != 0)
            flags = 0x40;
        else
            flags = 0;
        AppendMenu(hmenu, flags, id, text);
    }
}
