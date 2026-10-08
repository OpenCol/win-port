/* PORT: the node's value pointer read as a pointer (deref_long read it as a long: 4 bytes, the same on i386; 8 on 64-bit Windows) (W9). */
/* list_prev: candidate for 1068:9f11 (107 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: list_prev
 *
 * VList_Next's mirror: steps the cursor back and returns the payload; NULL at the start or
 * with no cursor.

 * The stated size was 103 and the function is 107: `tools/re_agent/check_sizes.py`
 * found the extra bytes decoding as an unreachable epilogue, and the reconstruction
 * already grades over the whole extent. Only the number moved.
 */

#include "colonization.h"

#include "classes.h"
extern "C" VListItem *vlist_item_prev(VListItem *item);
/* same object: 4-byte `push cs; call ` (rule 47) */

void * list_prev(VList *list)
{
    if (list->cur != 0) {
        list->cur = vlist_item_prev(list->cur);
        if (list->cur == 0)
            return 0;
        else
            return *(void **)list->cur;
    } else
        return 0;
}
