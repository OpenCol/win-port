/* PORT: this file's own declarations of MSOpenFile removed: the header declares it from the definition (W1 step 3). */
/* file_MSOpenFile_1070_7e50: candidate for 1070:7e50 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSOpenFile with four words of arguments passed through.
 */

#include "colonization.h"

#include "records.h"

int file_MSOpenFile_1070_7e50(char *name, void *file)
{
    return MSOpenFile(name, (MSFile *)file);
}
