/* ResourceFile_dtor_1008_cfa3: candidate for 1008:cfa3 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ResourceFile::~ResourceFile()
 *
 * This module's copy: closes the resource DLL handle it holds.
 */

#include "colonization.h"

#include "classes.h"
extern "C" short MSOpenResFile(char *name);

ResourceFile::~ResourceFile()
{
    close_resource_dll(h);
}
