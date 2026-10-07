/* MSPositionFile: candidate for 1070:808d (72 bytes).  Verdict: RELOCATABLE.
 *
 * _llseek from the start of the file; 0 for no handle or failure, else 1.
 */

#include "colonization.h"

#include "records.h"

int MSPositionFile(MSFile *file, long off)
{
    if (file->handle == 0)
        return 0;
    if (_llseek(file->handle, off, 0) == -1L)
        return 0;
    else
        return 1;
}
