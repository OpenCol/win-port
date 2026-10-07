/* VList_Add_1060_693c: candidate for 1060:693c (34 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `resource_find_1040_a976`'s twin reaching `VList_Add` instead of
 * `MSGetNamedResource`: two pointers passed straight through.
 *
 * Four bytes shorter than the twin because the `return` is not written out (rule 44). The
 * comparator masks the relocated call word, so the callee's name comes from the
 * disassembly.
 *
 * RENAMED 2026-09-26 from VList_add_1060_693c: the Mac build's VList::Add(void*) -- module 1060's pass-through copy to VList_Add, keeping its address suffix. The width matches.
 */

#include "colonization.h"

#include "classes.h"
extern void VList_Add(void *list, void *item);

void VList_Add_1060_693c(VList *list, void *item)
{
    VList_Add(list, item);
}
