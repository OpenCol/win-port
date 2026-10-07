/* menu_find_item: candidate for 1070:6871 (188 bytes).  Verdict: RELOCATABLE.
 *
 * Searches a menu tree for item id: recursing into submenus (GetMenuItemID -1), it returns
 * (position + 1) << 16 | (depth - 1) for the hit, 0 when absent; the depth counter at
 * DGROUP:0x56de is incremented on every level and never decremented here.
 */

#include "colonization.h"

long menu_find_item(unsigned hmenu, unsigned id)
{
    long r = 0;
    int item;
    int item2;
    unsigned sub;
    int count;
    int i;

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
