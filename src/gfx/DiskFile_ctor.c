/* DiskFile_ctor: candidate for 1068:9285 (73 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DiskFile::DiskFile()
 *
 * The constructor of the 0x8c-byte DiskFile record (Flic's member at +0xa6): the whole
 * object zeroed.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

DiskFile::DiskFile()
{
    memset_rtl(this, 0, 0x8c);
}
