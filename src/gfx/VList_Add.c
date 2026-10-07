/* VList_Add: candidate for 1068:9fa0 (275 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_Add
 * name-source: sibling-build
 *
 * Appends a value to a VList: a new VListItem carries it; an empty list gets it as first, last
 * and current item, otherwise it is linked after the last item, which it becomes (and the
 * current item follows). The module's one-line accessors and VListItem's constructor are
 * defined here because the shipped calls to them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from VList_add_1068_9fa0: the Mac build's VList::Add(void*): this plus a value, appended in a new VListItem. The width matches.
 */

#include "colonization.h"

#include "classes.h"

extern "C" int VList_IsEmpty(VList *l);

extern "C" void VListItem_SetListItem(VListItem *it, void *item);

extern "C" void VListItem_SetPrev(VListItem *it, VListItem *p);

extern "C" void VListItem_SetNext(VListItem *it, VListItem *n);

void VList_Add(VList *list, void *item)
{
    VListItem *it;

    it = new VListItem;
    if (VList_IsEmpty(list)) {
        list->first = it;
        list->last = it;
        list->cur = it;
        VListItem_SetListItem(it, item);
    } else {
        VListItem_SetListItem(it, item);
        VListItem_SetPrev(it, list->last);
        VListItem_SetNext(list->last, it);
        list->last = it;
        list->cur = list->last;
    }
}

