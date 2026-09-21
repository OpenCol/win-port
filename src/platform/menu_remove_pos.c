/* menu_remove_pos -- shipped at 1070:d72b.
 *
 * Ported by tools/port.py from matched/platform/menu_remove_pos.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DeleteMenu(col_u16 hmenu, col_u16 pos, col_u16 flags);;
extern col_i16 pascal RemoveMenu(col_u16 hmenu, col_u16 pos, col_u16 flags);;
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);;
#endif

void menu_remove_pos(col_u16 hmenu, col_i16 pos, col_i16 item)
{
    col_u16 sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        if (item == -1)
            RemoveMenu(hmenu, pos - 1, 0x400);
        else
            DeleteMenu(sub, item - 1, 0x400);
    }
}
