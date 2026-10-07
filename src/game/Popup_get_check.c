/* PORT: this file's own declarations of Popup_find_item removed: the header declares it from the definition (W1 step 3). */
/* Popup_get_check: candidate for 1050:0f15 (81 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_get_check
 * name-source: sibling-build
 *
 * Whether the list item with the given key has flag 4 set; 0 when no item has that key.
 *
 * RENAMED 2026-09-26 from item_flag4_get: bit 4 of a popup item's flags, the checked state: game_menu_debug
 * and run_text_dialog read their check boxes back through it.
 *
 * RENAMED 2026-09-26 from popup_item_checked: the Mac build's Popup::get_check(int): this plus an item key, reading its check mark. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
int Popup_get_check(Popup *popup, int key)
{
    PopupItem *item;
    int r = 0;
    item = Popup_find_item(popup, key);
    if (item != 0)
        r = (item->flags & 4) != 0;
    return r;
}
