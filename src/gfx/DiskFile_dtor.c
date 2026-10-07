/* DiskFile_dtor: candidate for 1068:9332 (65 bytes, 48 callers).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DiskFile::~DiskFile()
 *
 * The destructor of DiskFile: closes through the module's close routine and nothing else.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */

DiskFile::~DiskFile()
{
    MSCloseFile_1068_95b5(this);
}
