/* file_MSCreateFile_1050_9174: candidate for 1050:9174 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSCreateFile with its two -pointer arguments swapped.
 */

#include "colonization.h"

#include "records.h"
extern int MSCreateFile(const char *name, MSFile *file);

int file_MSCreateFile_1050_9174(void *file, void *name)
{
    return MSCreateFile((const char *)name, (MSFile *)file);
}
