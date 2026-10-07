/* MSSeekFile: candidate for 1070:803f (72 bytes).  Verdict: RELOCATABLE.
 *
 * _llseek relative to the current position; 0 for no handle or failure, else 1.
 */

#include "colonization.h"

#include "records.h"

int MSSeekFile(MSFile *file, long off)
{
    if (file->handle == 0)
        return 0;
    if (_llseek(file->handle, off, 1) == -1L)
        return 0;
    else
        return 1;
}
