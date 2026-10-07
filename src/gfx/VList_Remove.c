/* VList_Remove: candidate for 1068:a3a6 (423 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_Remove
 * name-source: sibling-build
 *
 * Unlinks a VList's CURRENT item, deletes it, and makes a neighbour current -- the next item
 * when there is one, otherwise the previous. `VList_ClearList` calls it until `cur` is
 * null. An empty list or a null `cur` returns at once.
 *
 * The four cases are the four link shapes: the only item (all three list pointers cleared),
 * the first item (the next one becomes `first` and loses its `prev`), the last item (the
 * previous one becomes `last` and loses its `next`), and a middle item (its neighbours are
 * linked to each other). The links are read and written through the module's one-line
 * accessors rather than directly, and `first`/`last` are re-read through the list after being
 * set.
 *
 * `delete l->cur` is Borland's non-virtual delete: `INC DWORD SS:[0x10]` -- one, because
 * VListItem has no destructible members (rule 49) -- then the destructor with flag 3, and no
 * null test at the call site, since the destructor guards `this` itself. The destructor and
 * the five accessors come through the 4-byte `PUSH CS` form and are defined here (rule 47);
 * the early `return` has its own epilogue (rule 44).
 *
 * RENAMED 2026-09-26 from list_remove_current: the Mac build's VList::Remove(void): this only, removing the current item. The width matches.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` */

extern "C" int VList_IsEmpty(VList *l);
extern "C" VListItem *vlist_item_prev(VListItem *it);
extern "C" VListItem *vlist_item_next(VListItem *it);
extern "C" void VListItem_SetPrev(VListItem *it, VListItem *p);
extern "C" void VListItem_SetNext(VListItem *it, VListItem *n);

void VList_Remove(VList *list)
{
    VListItem *prev;
    VListItem *next;

    if (list->cur == 0 || VList_IsEmpty(list))
        return;
    prev = vlist_item_prev(list->cur);
    next = vlist_item_next(list->cur);
    if (prev == 0 && next == 0) {
        delete list->cur;
        list->first = 0;
        list->last = 0;
        list->cur = 0;
    } else if (prev == 0) {
        list->first = next;
        VListItem_SetPrev(list->first, 0);
        delete list->cur;
        list->cur = list->first;
    } else if (next == 0) {
        list->last = prev;
        VListItem_SetNext(list->last, 0);
        delete list->cur;
        list->cur = list->last;
    } else {
        VListItem_SetNext(prev, next);
        VListItem_SetPrev(next, prev);
        delete list->cur;
        list->cur = next;
    }
}

