/* save_read_field: candidate for 1008:d834 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Save-file wrapper: MSReadFile(f, buf, n) with the length as a long.
 */

#include "colonization.h"

#include "records.h"
extern int MSReadFile(MSFile *file, void *buf, long n);

int save_read_field(void *file, void *buf, long n)
{
    return MSReadFile((MSFile *)file, buf, n);
}
