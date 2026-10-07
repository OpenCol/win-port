/* Popup_find_item: candidate for 1050:0d41 (102 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * The first node of a list whose key field matches, or a null pointer when the list runs out.
 *
 * RENAMED 2026-09-26 from list_find_by_key: the Mac build's Popup::find_item(int): this plus a key, the item lookup every per-item setter (grey, hilite, set_check) goes through. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
PopupItem * Popup_find_item(Popup *popup, int key)
{
    PopupItem *found = 0;
    PopupItem *item = popup->items;
    while (item != 0 && found == 0) {
        if (item->id == key)
            found = item;
        item = item->next;
    }
    return found;
}
