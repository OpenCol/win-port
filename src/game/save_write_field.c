/* save_write_field: candidate for 1008:d8c4 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Save-file wrapper: MSWriteFile(f, buf, n) with the length as a long.
 */

#include "colonization.h"

#include "records.h"
extern int MSWriteFile(void *file, void *buf, long n);

int save_write_field(void *file, void *buf, long n)
{
    return MSWriteFile((MSFile *)file, buf, n);
}
