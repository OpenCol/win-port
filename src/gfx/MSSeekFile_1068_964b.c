/* MSSeekFile_1068_964b: candidate for 1068:964b (36 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MSSeekFile_1068_964b
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
#include "classes.h"
extern int MSSeekFile(MSFile *file, long pos);

int MSSeekFile_1068_964b(DiskFile *file, long pos)
{
    return MSSeekFile(&file->msfile, pos);
}
