/* Popup_set_check: candidate for 1050:0f66 (71 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_set_check
 * name-source: sibling-build
 *
 * Sets or clears flag 4 on the list item with the given key, doing nothing when no item has that key.
 *
 * RENAMED 2026-09-26 from item_flag4_set: sets or clears bit 4 of a popup item's flags, the checked state:
 * Popup_make_controls seeds each check box from it and game_menu_debug writes a bitmask into it.
 *
 * RENAMED 2026-09-26 from popup_item_set_checked: the Mac build's Popup::set_check(int, int): this plus an item key and on/off, setting its check mark. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern PopupItem * Popup_find_item(void *popup, int key);
void Popup_set_check(Popup *popup, int key, int on)
{
    PopupItem *item;
    item = Popup_find_item(popup, key);
    if (item != 0) {
        if (on)
            item->flags |= 4;
        else
            item->flags &= ~4;
    }
}
