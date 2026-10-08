/* PORT: the node's value pointer read as a pointer (deref_long read it as a long: 4 bytes, the same on i386; 8 on 64-bit Windows) (W9). */
/* VList_Head: candidate for 1068:9dde (64 bytes, 4 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_Head
 * name-source: sibling-build
 *
 * Sets the list's cursor (+8) to its head (+0) and returns the head's payload; NULL for
 * an empty list.
 *
 * RENAMED 2026-09-26 from list_first: the Mac build's VList::Head(void): this only, rewinding the cursor to the head and returning its value. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

void * VList_Head(VList *list)
{
    if (list->first != 0) {
        list->cur = list->first;
        return *(void **)list->first;
    } else
        return 0;
}
