/* VListItem_SetNext: candidate for 1068:9cd4 (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VListItem_SetNext
 * name-source: sibling-build
 *
 * Stores a pointer into the slot at +0x04 of a record. The setter paired with the readers of +4 on the same records.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * RENAMED 2026-09-26 from set_next: the Mac build's VListItem::SetNext(void*): this plus a pointer, stored as the item's next link (+4) -- what vlist_item_next reads. The width matches.
 */

#include "colonization.h"

#include "classes.h"

void VListItem_SetNext(VListItem *it, VListItem *h)
{
    it->next = h;
}
