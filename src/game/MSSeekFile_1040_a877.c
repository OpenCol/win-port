/* MSSeekFile_1040_a877: candidate for 1040:a877 (36 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSSeekFile`, named for the routine it reaches:
 * the file record and a 32-bit position, the latter pushed as one `PUSH DWORD`.
 *
 * 36 bytes, not the census 32: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is a call and is `extern`.
 */

#include "colonization.h"

#include "records.h"
extern int MSSeekFile(MSFile *file, long pos);

int MSSeekFile_1040_a877(void *file, long pos)
{
    return MSSeekFile((MSFile *)file, pos);
}
