/* menu_insert_auto_id -- shipped at 1070:d77f.
 *
 * Ported by tools/port.py from matched/platform/menu_insert_auto_id.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned g_next_menu_id;                      DGROUP:0x39b8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal AppendMenu(col_u16 hmenu, col_u16 flags, col_u16 id, const char *text);
extern col_i16 pascal GetMenuItemCount(col_u16 hmenu);
extern col_i16 pascal InsertMenu(col_u16 hmenu, col_u16 pos, col_u16 flags, col_u16 id, const char *text);
extern col_u16 pascal CreatePopupMenu(void);
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);
#endif

void menu_insert_auto_id(col_u16 hmenu, col_i16 pos, col_i16 item, const char *text)
{
    col_u16 sub;
    col_i16 n;
    col_u16 flags;
    col_u16 popup;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        n = 0;
        if (sub != 0 && item != -1 && pos != -1)
            n = GetMenuItemCount(sub);
        if (n % 22 == 0 && n != 0)
            flags = 0x440;
        else
            flags = 0x400;
        if (item == -1) {
            popup = CreatePopupMenu();
            if (pos == -1)
                AppendMenu(hmenu, flags | 0x10, popup, text);
            else
                InsertMenu(hmenu, pos - 1, flags | 0x10, popup, text);
        } else
            InsertMenu(sub, item - 1, flags, g_next_menu_id++, text);
    }
}
