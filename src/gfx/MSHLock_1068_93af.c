/* MSHLock_1068_93af: candidate for 1068:93af (29 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 29, not the census 25. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `MSHLock`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void *MSHLock_1068_93af(int handle)
{
    return MSHLock(handle);
}
