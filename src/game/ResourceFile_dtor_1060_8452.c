/* PORT: this file's own declarations of MSOpenResFile removed: the header declares it from the definition (W1 step 3). */
/* ResourceFile_dtor_1060_8452: candidate for 1060:8452 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ResourceFile::~ResourceFile()
 *
 * This module's copy: closes the resource DLL handle it holds.
 */

#include "colonization.h"

#include "classes.h"

ResourceFile::~ResourceFile()
{
    close_resource_dll(h);
}
