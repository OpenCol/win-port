/* MSReadFile: candidate for 1070:7fed (82 bytes).  Verdict: RELOCATABLE.
 *
 * _hread(handle, buf, n); 0 for no handle or failure, else 1.
 */

#include "colonization.h"

#include "records.h"

int MSReadFile(MSFile *file, void *buf, long n)
{
    if (file->handle == 0)
        return 0;
    if (_hread(file->handle, buf, n) == -1L)
        return 0;
    else
        return 1;
}
