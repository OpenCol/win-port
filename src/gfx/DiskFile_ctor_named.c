/* PORT: this file's own declarations of MSCreateFile, MSOpenFile removed: the header declares them from the definition (W1 step 3). */
/* DiskFile_ctor_named: candidate for 1068:92ce (100 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: DiskFile::DiskFile(char*,int)
 *
 * DiskFile's constructor with a name: mode 1 opens the existing file (MSOpenFile), anything
 * else creates it (MSCreateFile). The module's two file wrappers are defined here because the
 * shipped calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

extern "C" extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

DiskFile::DiskFile(char *name, int mode)
{
    if (mode == 1)
        file_MSOpenFile_1068_95d5(this, name);
    else
        MSCreateFile_1068_9565(this, name);
}
