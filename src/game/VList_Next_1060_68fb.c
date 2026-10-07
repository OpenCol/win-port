/* PORT: this file's own declarations of VList_Next removed: the header declares it from the definition (W1 step 3). */
/* VList_Next_1060_68fb: candidate for 1060:68fb (32 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 32, not the census 28. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `VList_Next`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from list_next_1060_68fb: the Mac build's VList::Next(void) -- module 1060's copy, keeping its address suffix. The width matches.
 */

#include "colonization.h"

#include "classes.h"

void *VList_Next_1060_68fb(VList *list)
{
    return VList_Next(list);
}
