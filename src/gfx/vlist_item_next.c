/* vlist_item_next: candidate for 1068:d61a (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: vlist_item_next
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The NEXT link of a VList item: the pointer at +0x4.
 * A one-line accessor, 20-odd bytes of prologue and epilogue around a single move.
 *
 * `VList_Remove` reads this one into `next` and 1068:d637 into `prev`, and
 * `VList_Next` walks the list with it. The two are adjacent in the segment.
 */

#include "colonization.h"

#include "classes.h"

VListItem *vlist_item_next(VListItem *item)
{
    return item->next;
}
