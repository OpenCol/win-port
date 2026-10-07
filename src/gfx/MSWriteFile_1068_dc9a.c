/* MSWriteFile_1068_dc9a: candidate for 1068:dc9a (42 bytes).  Verdict: RELOCATABLE.
 *
 * Save-file wrapper: MSWriteFile(f, buf, n) with the length as a long.
 *
 * TRANSFERRED from `save_read_field` (1008:d834), byte-identical except for the one relocated call:
 * this copy reaches `MSWriteFile` where that one reaches `MSReadFile`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 */

#include "colonization.h"

#include "records.h"
extern int MSWriteFile(void *file, void *buf, long n);

int MSWriteFile_1068_dc9a(void *out, void *buf, long n)
{
    return MSWriteFile((MSFile *)out, buf, n);
}
