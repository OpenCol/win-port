/* PORT: this file's own declarations of MSFileSize removed: the header declares it from the definition (W1 step 3). */
/* MSFileSize_1068_dbe6: candidate for 1068:dbe6 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 32, not the census 28. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `MSFileSize`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

#include "records.h"

long MSFileSize_1068_dbe6(void *p)
{
    return MSFileSize((MSFile *)p);
}
