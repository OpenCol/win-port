/* PORT: this file's own declarations of VList_Head, VList_Next removed: the header declares them from the definition (W1 step 3). */
/* list_find_item: candidate for 1068:a2ef (101 bytes).  Verdict: RELOCATABLE.
 *
 * Walks the list with VList_Head / VList_Next for a given payload; returns it or NULL.
 */

#include "colonization.h"

#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
void * list_find_item(void *l, void *item)
{
    void *p;

    p = VList_Head((VList *)l);
    while (p != 0) {
        if (p == item)
            return p;
        p = VList_Next((VList *)l);
    }
    return 0;
}
