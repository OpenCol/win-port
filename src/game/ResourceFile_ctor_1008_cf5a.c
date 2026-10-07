/* PORT: this file's own declarations of MSOpenResFile removed: the header declares it from the definition (W1 step 3). */
/* ResourceFile_ctor_1008_cf5a: candidate for 1008:cf5a (73 bytes).  Verdict: RELOCATABLE.
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
