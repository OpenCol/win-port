/* ResourceFile_dtor_1010_2867: candidate for 1010:2867 (66 bytes).  Verdict: RELOCATABLE.
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
