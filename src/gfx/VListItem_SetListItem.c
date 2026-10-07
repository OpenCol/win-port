/* VListItem_SetListItem: candidate for 1068:9d08 (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VListItem_SetListItem
 * name-source: sibling-build
 *
 * Stores a pointer in a list item's first field and nothing else -- the 32-bit move is
 * `db 0x66,0x26,0x89,0x07`, an operand-size prefix on `mov [es:bx],eax`.
 *
 * `VList_Add` and `VList_insert` both call it with the value they were
 * handed, and both already write the body as their in-file stub: `it->value = v`.
 *
 * RENAMED 2026-09-26 from vlist_item_set_value: the Mac build's VListItem::SetListItem(void*): this plus a value, stored in the item's first field. The width matches.
 */

#include "colonization.h"

#include "classes.h"

void VListItem_SetListItem(VListItem *it, void *item)
{
    it->value = item;
}
