/* PORT: this file's own declarations of Popup_find_item removed: the header declares it from the definition (W1 step 3). */
/* Popup_grey: candidate for 1050:0e0d (71 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_grey
 * name-source: sibling-build
 *
 * Sets or clears flag 1 on the list item with the given key, doing nothing when no item has that key.
 *
 * RENAMED 2026-09-26 from item_flag1_set: sets or clears bit 1 of a popup item's flags, which greys it out:
 * the Europe dialogs set it on a unit the treasury cannot pay for, orders_road and orders_clear
 * on an unaffordable option, ask_port on the current destination, and
 * Popup_make_controls disables a check box (checkbox_disable) for an item that has it.
 *
 * RENAMED 2026-09-26 from popup_item_set_disabled: the Mac build's Popup::grey(int, int): this plus an item key and on/off, greying the item. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
void Popup_grey(Popup *popup, int key, int on)
{
    PopupItem *item;
    item = Popup_find_item(popup, key);
    if (item != 0) {
        if (on)
            item->flags |= 1;
        else
            item->flags &= ~1;
    }
}
