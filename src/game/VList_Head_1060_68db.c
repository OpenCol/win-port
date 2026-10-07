/* VList_Head_1060_68db: candidate for 1060:68db (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 32, not the census 28. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `VList_Head`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from list_first_1060_68db: the Mac build's VList::Head(void) -- module 1060's copy, keeping its address suffix. The width matches.
 */

#include "colonization.h"

#include "classes.h"
extern void *VList_Head(void *l);

void *VList_Head_1060_68db(VList *list)
{
    return VList_Head(list);
}
