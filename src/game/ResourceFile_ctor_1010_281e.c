/* PORT: this file's own declarations of MSOpenResFile removed: the header declares it from the definition (W1 step 3). */
/* ResourceFile_ctor_1010_281e: candidate for 1010:281e (73 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ResourceFile::ResourceFile(char*)
 *
 * This module's copy: opens the named resource DLL and keeps its handle (2-byte object).
 */

#include "colonization.h"

#include "classes.h"

ResourceFile::ResourceFile(char *name)
{
    h = MSOpenResFile(name);
}
