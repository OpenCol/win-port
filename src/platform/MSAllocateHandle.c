/* MSAllocateHandle: candidate for 1070:d0c6 (50 bytes).  Verdict: RELOCATABLE.
 *
 * GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, size); a null result is reported through
 * error_code_set(1). There is no return statement: the caller gets whatever AX holds --
 * the handle on success (the local is tested from the stack and never reloaded).
 */

#include "colonization.h"

#include "grerror.h"

extern void error_code_set(int code);

unsigned MSAllocateHandle(unsigned long size)
{
    unsigned h = GlobalAlloc(0x42, size);

    if (h == 0)
        error_code_set(GRERR_ALLOC_FAILED);
}
