/* menu_check_item: candidate for 1070:da48 (53 bytes).  Verdict: RELOCATABLE.
 *
 * Checks (MF_CHECKED, 8) or unchecks a menu item by command id when the menu handle is
 * non-null; the command id is passed as a long.
 */

#include "colonization.h"

void menu_check_item(unsigned hmenu, unsigned long id, char check)
{
    if (hmenu != 0) {
        if (check == 0)
            CheckMenuItem(hmenu, (unsigned)id, 0);
        else
            CheckMenuItem(hmenu, (unsigned)id, 8);
    }
}
