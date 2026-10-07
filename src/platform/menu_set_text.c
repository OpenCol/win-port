/* menu_set_text: candidate for 1070:d926 (112 bytes).  Verdict: RELOCATABLE.
 *
 * Renames a menu entry: with item -1 the popup itself (position `pos` - 1 of the menu bar,
 * MF_BYPOSITION|MF_POPUP), otherwise item `item` - 1 of that popup, keeping its command id.
 */

#include "colonization.h"

void menu_set_text(unsigned hmenu, int pos, int item, const char *text)
{
    unsigned sub;

    if (hmenu != 0) {
        sub = GetSubMenu(hmenu, pos - 1);
        if (item == -1)
            ModifyMenu(hmenu, pos - 1, 0x410, sub, text);
        else
            ModifyMenu(sub, item - 1, 0x400, GetMenuItemID(sub, item - 1), text);
    }
}
