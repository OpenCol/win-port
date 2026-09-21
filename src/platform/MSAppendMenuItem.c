/* MSAppendMenuItem -- shipped at 1070:dae6.
 *
 * Ported by tools/port.py from matched/platform/MSAppendMenuItem.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern char pascal AppendMenu(col_u16 hmenu, col_u16 flags, col_u16 id, char *s);;
extern col_i16 pascal GetMenuItemCount(col_u16 hmenu);;
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);;
#endif

void MSAppendMenuItem(col_u16 hmenu, col_i16 pos, char *text, col_u16 id)
{
    col_i16 n;
    char r;
    col_u16 flags;
    col_u16 sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        n = GetMenuItemCount(sub);
        if (n % 22 == 0 && n != 0)
            flags = 0x40;
        else
            flags = 0;
        r = AppendMenu(sub, flags, id, text);
    }
}
