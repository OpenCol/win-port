/* menu_set_text -- shipped at 1070:d926.
 *
 * Ported by tools/port.py from matched/platform/menu_set_text.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal ModifyMenu(col_u16 hmenu, col_u16 pos, col_u16 flags, col_u16 id, const char *text);
extern col_u16 pascal GetMenuItemID(col_u16 hmenu, col_i16 pos);
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);
#endif

void menu_set_text(col_u16 hmenu, col_i16 pos, col_i16 item, const char *text)
{
    col_u16 sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        if (item == -1)
            ModifyMenu(hmenu, pos - 1, 0x410, sub, text);
        else
            ModifyMenu(sub, item - 1, 0x400, GetMenuItemID(sub, item - 1), text);
    }
}
