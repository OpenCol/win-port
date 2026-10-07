/* file_MSOpenFile_1070_7e50: candidate for 1070:7e50 (38 bytes).  Verdict: RELOCATABLE.
 *
 * MSOpenFile with four words of arguments passed through.
 */

#include "colonization.h"

#include "records.h"
extern int MSOpenFile(char *name, MSFile *file);

int file_MSOpenFile_1070_7e50(char *name, void *file)
{
    return MSOpenFile(name, (MSFile *)file);
}
