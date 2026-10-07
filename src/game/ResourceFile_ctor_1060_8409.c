/* ResourceFile_ctor_1060_8409: candidate for 1060:8409 (73 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ResourceFile::ResourceFile(char*)
 *
 * Module 1060's copy: opens the named resource DLL and keeps its handle (2-byte object).
 */

#include "colonization.h"

#include "classes.h"
extern "C" short MSOpenResFile(char *name);

ResourceFile::ResourceFile(char *name)
{
    h = MSOpenResFile(name);
}
