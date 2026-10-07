/* menu_insert_1070_d855: candidate for 1070:d855 (209 bytes).  Verdict: RELOCATABLE.
 *
 * Inserts a menu entry: with `item` -1 a new popup (appended to the bar when `pos` is -1,
 * else inserted before position `pos` - 1); otherwise the command `id` before item `item` - 1
 * of popup `pos` - 1. A command inserted into a popup already holding a nonzero multiple of 22 entries starts a new column (MF_MENUBREAK, 0x40).
 */

#include "colonization.h"

void menu_insert_1070_d855(unsigned hmenu, int pos, int item, const char *text, unsigned id)
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
            InsertMenu(sub, item - 1, flags, id, text);
    }
}
