/* PORT: this file's own declarations of Popup_find_item removed: the header declares it from the definition (W1 step 3). */
/* Popup_hilite: candidate for 1050:0e54 (71 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_hilite
 * name-source: sibling-build
 *
 * Sets or clears flag 2 on the list item with the given key, doing nothing when no item has that key.
 *
 * RENAMED 2026-09-26 from item_flag2_set: sets or clears bit 2 of a popup item's flags, the preselected item:
 * colony_select_profession marks the colonist's present specialty, ColonyWin_colony_hammer_change the colony's
 * current build, or "(More)" when nothing else is preselected -- each beside Popup_set_active.
 *
 * RENAMED 2026-09-26 from popup_item_set_current: the Mac build's Popup::hilite(int, int): this plus an item key and on/off, marking the preselected item. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
void Popup_hilite(Popup *popup, int key, int on)
{
    PopupItem *item;
    item = Popup_find_item(popup, key);
    if (item != 0) {
        if (on)
            item->flags |= 2;
        else
            item->flags &= ~2;
    }
}
