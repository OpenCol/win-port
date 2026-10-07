/* MSHUnlock_1068_93cc: candidate for 1068:93cc (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `MSHUnlock`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void *MSHUnlock_1068_93cc(int handle)
{
    return MSHUnlock(handle);
}
