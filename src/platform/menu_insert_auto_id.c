/* menu_insert_auto_id: candidate for 1070:d77f (214 bytes).  Verdict: RELOCATABLE.
 *
 * menu_insert_1070_d855's sibling that numbers commands itself: the id comes from the counter
 * at DGROUP:0x39b8, incremented after use.
 */

#include "colonization.h"

void menu_insert_auto_id(unsigned hmenu, int pos, int item, const char *text)
{
    unsigned sub;
    int n;
    unsigned flags;
    unsigned popup;

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
