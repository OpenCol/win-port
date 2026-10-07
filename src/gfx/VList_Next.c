/* VList_Next: candidate for 1068:9ea6 (107 bytes, 3 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_Next
 * name-source: sibling-build
 *
 * Advances the list cursor (+8) and returns the new item's payload; NULL at the end or
 * when there is no cursor.

 * The stated size was 103 and the function is 107: `tools/re_agent/check_sizes.py`
 * found the extra bytes decoding as an unreachable epilogue, and the reconstruction
 * already grades over the whole extent. Only the number moved.
 *
 * RENAMED 2026-09-26 from list_next: the Mac build's VList::Next(void): this only, advancing the cursor and returning the new value. The width matches.
 */

#include "colonization.h"

#include "classes.h"
extern "C" VListItem *vlist_item_next(VListItem *item);
/* same object: 4-byte `push cs; call ` (rule 47) */

void * VList_Next(VList *list)
{
    if (list->cur != 0) {
        list->cur = vlist_item_next(list->cur);
        if (list->cur == 0)
            return 0;
        else
            return (void *)deref_long((long *)list->cur);
    } else
        return 0;
}
