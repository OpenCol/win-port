/* file_MSCreateFile_1070_7f21: candidate for 1070:7f21 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSCreateFile with four words of arguments passed through.
 */

#include "colonization.h"

#include "records.h"
extern int MSCreateFile(char *name, MSFile *file);

int file_MSCreateFile_1070_7f21(char *name, void *file)
{
    return MSCreateFile(name, (MSFile *)file);
}
