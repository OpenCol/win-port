/* PORT: this file's own declarations of MSCreateFile removed: the header declares it from the definition (W1 step 3). */
/* file_MSCreateFile_1070_7f21: candidate for 1070:7f21 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSCreateFile with four words of arguments passed through.
 */

#include "colonization.h"

#include "records.h"

int file_MSCreateFile_1070_7f21(char *name, void *file)
{
    return MSCreateFile(name, (MSFile *)file);
}
