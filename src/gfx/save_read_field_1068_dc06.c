/* save_read_field_1068_dc06: candidate for 1068:dc06 (42 bytes).  Verdict: RELOCATABLE.
 *
 * Save-file wrapper: MSReadFile(f, buf, n) with the length as a long.
 *
 * TRANSFERRED: byte-identical to `save_read_field` (1008:d834) and reaching the same routine, one of
 * 12 copies of this function in the image.
 */

#include "colonization.h"

#include "records.h"
extern int MSReadFile(MSFile *file, void *buf, long n);

int save_read_field_1068_dc06(void *file, void *buf, long n)
{
    return MSReadFile((MSFile *)file, buf, n);
}
