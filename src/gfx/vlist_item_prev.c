/* vlist_item_prev: candidate for 1068:d637 (29 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: vlist_item_prev
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the dead second epilogue.
 *
 * The PREVIOUS link of a VList item: the pointer at +0x8.
 * A one-line accessor, 20-odd bytes of prologue and epilogue around a single move.
 *
 * Two callers write its body out as `return it->prev;` over a `VListItem`, and
 * `VList_Remove` pairs it with `vlist_item_next` (1068:d61a).
 */

#include "colonization.h"

#include "classes.h"

VListItem *vlist_item_prev(VListItem *it)
{
    return it->prev;
}
