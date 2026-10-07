/* MSCloseFile: candidate for 1070:7fa6 (65 bytes).  Verdict: RELOCATABLE.
 *
 * _lclose on the file handle; 0 for no handle or a failed close, else the handle is
 * cleared and 1 returned.
 */

#include "colonization.h"

#include "records.h"

int MSCloseFile(MSFile *file)
{
    if (file->handle == 0)
        return 0;
    if (_lclose(file->handle) == -1)
        return 0;
    else {
        file->handle = 0;
        return 1;
    }
}
