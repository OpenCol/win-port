/* ResourceFile_ctor_1008_cf5a: candidate for 1008:cf5a (73 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ResourceFile::ResourceFile(char*)
 *
 * This module's copy: opens the named resource DLL and keeps its handle (2-byte object).
 */

#include "colonization.h"

#include "classes.h"
extern "C" short MSOpenResFile(char *name);

ResourceFile::ResourceFile(char *name)
{
    h = MSOpenResFile(name);
}
