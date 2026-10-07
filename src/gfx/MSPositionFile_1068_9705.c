/* MSPositionFile_1068_9705: candidate for 1068:9705 (36 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSPositionFile`, named for the routine it reaches:
 * the file record and a 32-bit position, the latter pushed as one `PUSH DWORD`.
 *
 * 36 bytes, not the census 32: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is a call and is `extern`.
 */

#include "colonization.h"

#include "records.h"
extern int MSPositionFile(void *file, long pos);

int MSPositionFile_1068_9705(void *file, long pos)
{
    return MSPositionFile((MSFile *)file, pos);
}
