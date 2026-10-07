/* VListItem_SetPrev: candidate for 1068:9cee (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VListItem_SetPrev
 * name-source: sibling-build
 *
 * Stores a pointer into the slot at +0x08 of a record. Its twin four bytes along.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * RENAMED 2026-09-26 from set_prev: the Mac build's VListItem::SetPrev(void*): this plus a pointer, stored as the item's previous link (+8) -- what vlist_item_prev reads. The width matches.
 */

#include "colonization.h"

#include "classes.h"

void VListItem_SetPrev(VListItem *it, VListItem *h)
{
    it->prev = h;
}
