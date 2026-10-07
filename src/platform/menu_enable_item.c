/* menu_enable_item: candidate for 1070:da13 (53 bytes).  Verdict: RELOCATABLE.
 *
 * Enables (flags 0) or greys (MF_GRAYED|MF_DISABLED, 3) a menu item by command id when the
 * menu handle is non-null; the command id is passed as a long.
 *
 * RENAMED 2026-09-27 from menu_enable_item_1070_da13: the address suffix went with the family: its two MenuClass-level copies are now MSEnableItem_*, so this hmenu-level routine is the only menu_enable_item.
 */

#include "colonization.h"

void menu_enable_item(unsigned hmenu, unsigned long id, char enable)
{
    if (hmenu != 0) {
        if (enable == 0)
            EnableMenuItem(hmenu, (unsigned)id, 3);
        else
            EnableMenuItem(hmenu, (unsigned)id, 0);
    }
}
