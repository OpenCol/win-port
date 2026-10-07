/* clear_list_bit_1: candidate for 1050:0e9b (61 bytes).  Verdict: EXACT.
 *
 * Walks the list whose head is the pointer at +0x36e of the record, the
 * next pointer at +0xa of each item, clearing bit 1 of each item's first
 * word.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void clear_list_bit_1(Popup *popup)
{
    PopupItem *item;

    for (item = popup->items; item != 0; item = item->next)
        item->flags &= ~1;
}
