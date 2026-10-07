/* MSCloseFile_1050_90e6: candidate for 1050:90e6 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 32, not the census 28. The trailing 4 bytes are the SECOND epilogue that
 * the `return` adds (rule 44), which Ghidra excludes because nothing reaches it.
 *
 * This module's one-line forwarder to `MSCloseFile`, named for the routine it reaches.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

#include "records.h"
extern int MSCloseFile(MSFile *file);

int MSCloseFile_1050_90e6(void *p)
{
    return MSCloseFile((MSFile *)p);
}
