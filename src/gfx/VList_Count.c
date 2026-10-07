/* PORT: this file's own declarations of VList_Head, VList_Next removed: the header declares them from the definition (W1 step 3). */
/* VList_Count: candidate for 1068:a354 (82 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_Count
 * name-source: sibling-build
 *
 * Walks the list with VList_Head / VList_Next and counts the items.
 *
 * RENAMED 2026-09-26 from list_count: the Mac build's VList::Count(void): this only, counting the items with VList_Head/VList_Next. The width matches.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
int VList_Count(VList *l)
{
    int n = 0;
    void *p;

    p = VList_Head(l);
    while (p != 0) {
        n++;
        p = VList_Next(l);
    }
    return n;
}
