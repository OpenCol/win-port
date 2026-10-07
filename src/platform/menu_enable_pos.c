/* menu_enable_pos: candidate for 1070:d658 (125 bytes).  Verdict: RELOCATABLE.
 *
 * Enables (MF_BYPOSITION|MF_ENABLED, 0x400) or greys (0x403) a menu entry by position: with
 * `item` 0 the popup `pos` - 1 of the menu bar itself, otherwise item `item` - 1 of that popup.
 */

#include "colonization.h"

void menu_enable_pos(unsigned hmenu, int pos, int item, char enable)
{
    unsigned sub;

    if (item == 0) {
        if (enable == 0)
            EnableMenuItem(hmenu, pos - 1, 0x403);
        else
            EnableMenuItem(hmenu, pos - 1, 0x400);
    } else {
        sub = GetSubMenu(hmenu, pos - 1);
        if (enable == 0)
            EnableMenuItem(sub, item - 1, 0x403);
        else
            EnableMenuItem(sub, item - 1, 0x400);
    }
}
