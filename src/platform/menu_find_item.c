/* menu_find_item -- shipped at 1070:6871.
 *
 * Ported by tools/port.py from matched/platform/menu_find_item.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_menu_depth;                             DGROUP:0x56de
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal GetMenuItemCount(col_u16 hmenu);;
extern col_i16 pascal GetMenuItemID(col_u16 hmenu, col_i16 pos);;
extern col_u16 pascal GetSubMenu(col_u16 hmenu, col_i16 pos);;
#endif

col_i32 menu_find_item(col_u16 hmenu, col_u16 id)
{
    col_i32 r = 0;
    col_i16 item;
    col_i16 item2;
    col_u16 sub;
    col_i16 count;
    col_i16 i;

    g_menu_depth++;
    count = GetMenuItemCount(hmenu);
    for (i = 0; i < count; i++) {
        item = GetMenuItemID(hmenu, i);
        if (item == -1) {
            sub = GetSubMenu(hmenu, i);
            r = menu_find_item(sub, id);
            if (r != 0)
                i = count + 1;
        } else {
            item2 = GetMenuItemID(hmenu, i);
            if (item2 == id) {
                r = i + 1;
                r = r << 16;
                r |= g_menu_depth - 1;
                i = count + 1;
            }
        }
    }
    return r;
}
