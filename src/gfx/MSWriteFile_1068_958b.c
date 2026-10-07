/* PORT: this file's own declarations of MSWriteFile removed: the header declares it from the definition (W1 step 3). */
/* MSWriteFile_1068_958b: candidate for 1068:958b (42 bytes).  Verdict: RELOCATABLE.
 *
 * Save-file wrapper: MSWriteFile(f, buf, n) with the length as a long.
 *
 * TRANSFERRED from `save_read_field` (1008:d834), byte-identical except for the one relocated call:
 * this copy reaches `MSWriteFile` where that one reaches `MSReadFile`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

#include "records.h"

int MSWriteFile_1068_958b(void *file, void *buf, long n)
{
    return MSWriteFile((MSFile *)file, buf, n);
}
