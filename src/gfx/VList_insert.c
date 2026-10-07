/* VList_insert: candidate for 1068:a0f6 (505 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_insert
 *
 * INSERTS A VALUE BEFORE THE LIST'S CURRENT ITEM, and makes it current. Nothing calls it, but it is the
 * companion of `VList_Add`, which appends, and it fixes the rest of the list's shape.
 *
 * An empty list, or one with no current item, is just appended to. Inserting before the FIRST item also
 * moves the head. Otherwise the new item is spliced between the current item and the one before it, and
 * the six link writes go through the same four one-line accessors the rest of the list code uses.
 *
 * Each `new VListItem` gets its own stack temp, which is why the frame has two.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int VList_IsEmpty(VList *l);
extern "C" void VList_Add(VList *l, void *item);
extern "C" void VListItem_SetListItem(VListItem *it, void *item);
extern "C" void VListItem_SetPrev(VListItem *it, VListItem *p);
extern "C" void VListItem_SetNext(VListItem *it, VListItem *n);
extern "C" VListItem *vlist_item_prev(VListItem *it);

void VList_insert(VList *list, void *item)
{
    VListItem *it;
    VListItem *prev;

    if (VList_IsEmpty(list)) {
        VList_Add(list, item);
    } else if (list->cur == 0) {
        VList_Add(list, item);
    } else if (list->cur == list->first) {
        it = new VListItem;
        VListItem_SetPrev(list->cur, it);
        VListItem_SetListItem(it, item);
        VListItem_SetNext(it, list->cur);
        list->cur = it;
        list->first = list->cur;
    } else {
        it = new VListItem;
        prev = vlist_item_prev(list->cur);
        VListItem_SetNext(prev, it);
        VListItem_SetPrev(list->cur, it);
        VListItem_SetListItem(it, item);
        VListItem_SetPrev(it, prev);
        VListItem_SetNext(it, list->cur);
        list->cur = it;
    }
}

